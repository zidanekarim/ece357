#include "myshell.h"


int myshell_loop(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t linelen;
    char *token;
    char *delimiter = " \t\r\n\a";
    char **args;
    int run = 1;

    while (run) {
        printf("myshell> ");
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
                                                                                                                        

        args = malloc((linelen / 2 + 1) * sizeof(char*)); // Rough estimate of max args
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

        // run command
        myshell_execute(args);

        free(args);
    }
    free(line);
    return 0;
}

int myshell_execute(char **args) {
    if (args[0] == NULL) {
        errno = EINVAL;
        return 1; 
    }

    pid_t pid = fork();
    if (pid == 0) {
        // child 

        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "myshell: command not found: %s\n", args[0]);
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        fprintf(stderr, "Error in creating child process for command %s\n", args[0]);
    } else {
        // Parent process
        int status;
        while (waitpid(pid, &status, WUNTRACED) == -1) {
            if (errno != EINTR) {
                fprintf(stderr, "Error waiting for child process for command %s\n", args[0]);
                break;
            }
        }
    }
    return 1;
}