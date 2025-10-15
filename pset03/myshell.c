#include "myshell.h"


char* file_parser(char* input_str) { // extracts the file name from a redirection string
    // assumes input_str starts with a redirection operator
    char *delimiter = " \t\r\n\a";
    printf("Parsing file from string: %s\n", input_str);
    char *token;
    token = strtok(input_str, delimiter); // filename
    return token;
}


int myshell_loop(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t linelen;
    char *token;
    char *delimiter = " \t\r\n\a";
    char **args;
    int run = 1;
    int last_status = 0;

    while (run) {
        printf("myshell: ");
        linelen = getline(&line, &bufsize, stdin);
        if (linelen == -1) {
            if (feof(stdin)) { // means EOF (ctrl d)
                break; 
            } else {
                fprintf(stderr, "myshell: Error in reading input\n");
                continue;
            }
        }
        else if (line[0] == '#') {
            continue; // ignore comments
        }
                                                                                                                        

        args = malloc(ARG_MAX * sizeof(char*)); // Rough estimate of max args
        if (args == NULL) {
            fprintf(stderr, "myshell: allocation error\n");
            continue;
        }

        int position = 0;
        token = strtok(line, delimiter);
        while (token != NULL) {
            args[position++] = token;
            token = strtok(NULL, delimiter);
        }
        args[position] = NULL;

        // builtin commands 
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                errno = EINVAL;
                fprintf(stderr, "myshell: invalid argument to cd\n");
            } else {
                if ((last_status = chdir(args[1])) != 0) {
                    fprintf(stderr, "myshell: no such file or directory: %s\n", args[1]);
                }
            }
            free(args);
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            if (args[1] != NULL) {
                char* arg1 = args[1];
                free(args);
                int status = atoi(arg1);
                if (status == 0 && strcmp(arg1, "0") != 0) { // atoi returns 0 on error so check if atoi failed instead of input being 0
                    fprintf(stderr, "myshell: invalid argument to exit\n"); 
                    continue;
                }
                exit(status);
            }
            else {
                free(args);
                exit(last_status);
            }
            break;
        }
        else if (strcmp(args[0], "pwd") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                last_status = 0;
                printf("%s\n", cwd);
            } else {
                last_status = -1;
                errno = EINVAL;
                fprintf(stderr, "myshell: error retrieving current directory\n");
            }
            free(args);
            continue;
        }
        struct command* cmd = (struct command*) malloc(sizeof(struct command));
        if (cmd == NULL) {
            fprintf(stderr, "myshell: allocation error\n");
            free(args);
            continue;
        }
        cmd->args = args;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->error_file = NULL;
        cmd->append = 0;
        cmd->error_append = 0;
        // parsing redirections!!! (most fun part)
        for (int i = 0; args[i] != NULL; i++) {
            char* argument = args[i];
            char *filename = strtok(argument, "2<>"); 
           
            if (!filename) continue;

            if (argument[0] == '<') {
                cmd->input_file = filename;
            }
            else if (argument[0] == '>' && argument[1] != '>') {
                cmd->output_file = filename;
                cmd->append = 0;
            }
            else if (argument[0] == '>' && argument[1] == '>') {
                cmd->output_file = filename;
                cmd->append = 1;
            }
            else if (argument[0] == '2' && argument[1] == '>' && argument[2] != '>') {
                cmd->error_file = filename;
                cmd->error_append = 0;
            }
            else if (argument[0] == '2' && argument[1] == '>' && argument[2] == '>') {
                cmd->error_file = filename;
                cmd->error_append = 1;
            } else {
                continue; 
            }

            // skipping over these args in the args array because exec doesnt care
            for (int j = i; args[j] != NULL; j++) {
                args[j] = args[j + 1];
            }
            i--;
        }

                
        last_status = myshell_execute(cmd);
        free(cmd);

        free(args);
    }
    free(line);
    return 0;
}

int myshell_execute(struct command* cmd) {
    if (cmd->args[0] == NULL) {
        errno = EINVAL;
        return 1; 
    }
    // redirection handling
    int out_fd = STDOUT_FILENO;
    int err_fd = STDERR_FILENO;
    int in_fd = STDIN_FILENO;

    if (cmd->input_file != NULL) {
        // close STDIN_FILENO;



        in_fd = open(cmd->input_file, O_RDONLY);
        if (in_fd < 0) {
            fprintf(stderr, "myshell: cannot open input file %s\n", cmd->input_file);
            return -1;
        }
    }
    if (cmd->output_file != NULL) {
        if (cmd->append) {
            out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (out_fd < 0) {
            fprintf(stderr, "myshell: cannot open output file %s\n", cmd->output_file);
            return -1;
        }

    }
    if (cmd->error_file != NULL) {
        if (cmd->error_append) {
            err_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            err_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (err_fd < 0) {
            fprintf(stderr, "myshell: cannot open error file %s\n", cmd->error_file);
            return -1;
        }
        
    }


    pid_t pid = fork();
    if (pid == 0) {
        // child 
        if (dup2(err_fd, STDERR_FILENO) == -1) {
            fprintf(stderr, "myshell: error in error redirection for file %s\n", cmd->error_file);
            close(err_fd);
            return -1;
        }
        if (dup2(out_fd, STDOUT_FILENO) == -1) {
            fprintf(stderr, "myshell: error in output redirection for file %s\n", cmd->output_file);
            close(out_fd);
            return -1;
        }
        if (dup2(in_fd, STDIN_FILENO) == -1) {
            fprintf(stderr, "myshell: error in input redirection for file %s\n", cmd->input_file);
            close(in_fd);
            return -1;
        }


        if (execvp(cmd->args[0], cmd->args) == -1) {
            fprintf(stderr, "myshell: command not found: %s\n", cmd->args[0]);
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        fprintf(stderr, "Error in creating child process for command %s\n", cmd->args[0]);
    } else {
        // Parent process
        int status;
        while (waitpid(pid, &status, WUNTRACED) == -1) {
            if (errno != EINTR) {
                fprintf(stderr, "Error waiting for child process for command %s\n", cmd->args[0]);
                break;
            }
        }
    }
    return 1;
}