#include "myshell.h"


int exit_status; // make it global
bool custom_PS = false; //default to 'myshell: '

double time_parser(struct timeval time) {
    return (double) (time.tv_sec + time.tv_usec / 1000000.0);
}

double time_parser2(struct timeval time1, struct timeval time2)
{
    return (double) (time2.tv_sec - time1.tv_sec + (time2.tv_usec - time1.tv_usec) / 1000000.0);
}

int myshell_loop(FILE *input, bool interactive, int argc, char **argv)
{
    char *line = NULL;
    size_t bufsize = 0; // fails if not size_t
    ssize_t linelen;
    char *token;
    char *delimiter = " \t\r\n\a";
    char **args;
    int run = 1;
    int last_status = 0;
    glob_t wildcard_match;

    while (run)
    {
        if (interactive){   
            if(custom_PS){
                char *name = getenv("PS1");
                printf("%s ", name);
            }else{
                printf("myshell: ");
            }
        }

        linelen = getline(&line, &bufsize, input);
        line[strcspn(line, "\r\n")] = 0; // needed to add this to fix file input/script parsing
        if (linelen == -1)
        {
            if (feof(input))
            { // means EOF (ctrl d)
                break;
            }
            else
            {
                fprintf(stderr, "myshell: Error in reading input\n");
                continue;
            }
        }
        else if (line[0] == '#')
        {
            continue; // ignore comments
        }

        args = malloc(ARG_MAX * sizeof(char *));
        if (args == NULL)
        {
            fprintf(stderr, "myshell: allocation error\n");
            continue;
        }

        int position = 0;
        token = strtok(line, delimiter);
        while (token != NULL)
        {
            args[position++] = token;
            token = strtok(NULL, delimiter);
        }
        args[position] = NULL;

        //EC
        memset(&wildcard_match, 0, sizeof(glob_t));

        for (int i = 0; args[i] != NULL; i++){
            char *argument = args[i];      
            //EC(b) - Find any instance of $ followed by a number so parse through each token to find any starting with $ 
            // separated this from i/o parsing redirection since the token stripping was interfering >:(
            if (argument[0] == '$' && isdigit(argument[1])){
                if(!interactive){
                    int n = atoi(&argument[1]); // get position for corresponding shell script input
                        if (n + 1 < argc) {  // +1 for command
                            args[i] = argv[n + 1]; // actually change args[i] to contain whatever we inputted
                        }
                    }
            }

            //EC (d) 
            // separate the cases between (b) and (d) so we check to see if its a letter
            if (argument[0] == '$' && isalpha(argument[1])){
                char *env_var = argument + 1; // argument points to $xxx so we just want xxx
                char *env_val = getenv(env_var);
                // preventing NULL from being an argument (that would be bad thanks aidan for pointing it out)
                if (env_val != NULL){
                    args[i] = env_val;
                }else{
                    args[i] = "";
                }
            }

            // EC (e) by far the hardest one (understanding glob was a pain)
            // if (argument[0] == '?' || argument[0] == '*' || argument[0] == '['){   no work since then our prior cmds get yoinked 
            int flags;
            // On first iteration, need flag = 0 since APPEND should not be set on first invocation of glob()
            if (wildcard_match.gl_pathc == 0){
                flags = 0; 
            }else{
                flags = GLOB_APPEND;
            }
            //Append matching pathname or the actual string if no wildcard
            glob(args[i], flags | GLOB_NOCHECK, NULL, &wildcard_match);
            // }
        }

        //Reconstruct args to include all our matched things + unmatched things --> replace all previous free(args) w/ globfree()
        args = wildcard_match.gl_pathv; 

        // builtin commands
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                errno = EINVAL;
                fprintf(stderr, "myshell: invalid argument to cd\n");
            }
            else
            {
                if ((last_status = chdir(args[1])) != 0)
                {
                    fprintf(stderr, "myshell: no such file or directory: %s\n", args[1]);
                }
            }
            globfree(&wildcard_match);;
            continue;
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            if (args[1] != NULL)
            {
                char *arg1 = args[1];
                globfree(&wildcard_match);;
                int status = atoi(arg1);
                if (status == 0 && strcmp(arg1, "0") != 0)
                { // atoi returns 0 on error so check if atoi failed instead of input being 0
                    fprintf(stderr, "myshell: invalid argument to exit\n");
                    continue;
                }
                exit(status);
            }
            else
            {
                globfree(&wildcard_match);;
                exit(last_status);
            }
            break;
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                last_status = 0;
                printf("%s\n", cwd);
            }
            else
            {
                last_status = -1;
                errno = EINVAL;
                fprintf(stderr, "myshell: error retrieving current directory\n");
            }
            globfree(&wildcard_match);;
            continue;
        }

        // EC part (a) 
        else if ((strcmp(args[0], "echo") == 0) && (args[1] != NULL) && (strcmp(args[1], "$?")) == 0)
        {
            printf("%d\n", exit_status);
            globfree(&wildcard_match);;
            continue;
        }

        //EC part (c) - Must be in format of: export PS1='Whatever'
        else if ((strcmp(args[0], "export") == 0) && (args[1] != NULL))
        {
            custom_PS = true;

            //New PS1 --> split up the args
            char *PS_arg = args[1];  
            char *curr_PS1 = strtok(PS_arg, "=");
            char *new_PS = strtok(NULL, "=");

            setenv(curr_PS1, new_PS, 1);
            globfree(&wildcard_match);;
            continue;
        }

        struct command *cmd = (struct command *)malloc(sizeof(struct command));
        if (cmd == NULL)
        {
            fprintf(stderr, "myshell: allocation error\n");
            globfree(&wildcard_match);;
            continue;
        }
        cmd->args = args;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->error_file = NULL;
        cmd->append = 0;
        cmd->error_append = 0;

        // i/o parsing redirections!!! (most fun part)
        for (int i = 0; args[i] != NULL; i++)
        {
            char *argument = args[i];   
            char *filename = strtok(argument, "2<>");

            if (!filename)
                continue;

            if (argument[0] == '<')
            {
                cmd->input_file = filename;
            }
            else if (argument[0] == '>' && argument[1] != '>')
            {
                cmd->output_file = filename;
                cmd->append = 0; // for flags
            }
            else if (argument[0] == '>' && argument[1] == '>')
            {
                cmd->output_file = filename;
                cmd->append = 1;
            }
            else if (argument[0] == '2' && argument[1] == '>' && argument[2] != '>')
            {
                cmd->error_file = filename;
                cmd->error_append = 0;
            }
            else if (argument[0] == '2' && argument[1] == '>' && argument[2] == '>')
            {
                cmd->error_file = filename;
                cmd->error_append = 1;
            }
            else
            {
                continue;
            }

            // skipping over these args in the args array because exec doesnt care
            for (int j = i; args[j] != NULL; j++)
            {
                args[j] = args[j + 1];
            }
            i--;
        }

        last_status = myshell_execute(cmd);
        free(cmd);

        globfree(&wildcard_match);;
    }
    free(line);
    return 0;
}

int myshell_execute(struct command *cmd)
{
    if (cmd->args[0] == NULL)
    {
        errno = EINVAL;
        return 1;
    }
    // redirection handling
    int out_fd = STDOUT_FILENO;
    int err_fd = STDERR_FILENO;
    int in_fd = STDIN_FILENO;

    if (cmd->input_file != NULL)
    {
        // close STDIN_FILENO;

        in_fd = open(cmd->input_file, O_RDONLY);
        if (in_fd < 0)
        {
            fprintf(stderr, "myshell: cannot open input file %s\n", cmd->input_file);
            return -1;
        }
    }
    if (cmd->output_file != NULL)
    {
        if (cmd->append)
        {
            out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (out_fd < 0)
        {
            fprintf(stderr, "myshell: cannot open output file %s\n", cmd->output_file);
            return -1;
        }
    }
    if (cmd->error_file != NULL)
    {
        if (cmd->error_append)
        {
            err_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            err_fd = open(cmd->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (err_fd < 0)
        {
            fprintf(stderr, "myshell: cannot open error file %s\n", cmd->error_file);
            return -1;
        }
    }
    struct timeval start_time, end_time;
    int start_tm = gettimeofday(&start_time, NULL); // https://stackoverflow.com/questions/47215214/why-is-the-gettimeofday-timezone-wrong
    if (start_tm == -1)
    {
        fprintf(stderr, "Error in getting start time\n");
    }
    pid_t pid = fork();

    if (pid == 0)
    {
        // child
        if (dup2(err_fd, STDERR_FILENO) == -1)
        {
            fprintf(stderr, "myshell: error in error redirection for file %s\n", cmd->error_file);
            close(err_fd);
            _exit(EXIT_FAILURE);
        }
        if (dup2(out_fd, STDOUT_FILENO) == -1)
        {
            fprintf(stderr, "myshell: error in output redirection for file %s\n", cmd->output_file);
            close(out_fd);
            _exit(EXIT_FAILURE);
        }
        if (dup2(in_fd, STDIN_FILENO) == -1)
        {
            fprintf(stderr, "myshell: error in input redirection for file %s\n", cmd->input_file);
            close(in_fd);
            _exit(EXIT_FAILURE);
        }
        if (execvp(cmd->args[0], cmd->args) == -1)
        {
            fprintf(stderr, "myshell: command not found: %s\n", cmd->args[0]);
            exit(EXIT_FAILURE);
        }
        // exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "Error in creating child process for command %s\n", cmd->args[0]);
    }
    else
    {
        // Parent process
        int status;
        struct rusage usage;

        while (waitpid(pid, &status, WUNTRACED) == 0)
            ; // hanging here until child process ends

        int resources = getrusage(RUSAGE_CHILDREN, &usage);
        if (resources == -1)
            fprintf(stderr, "Error in getting resource usage\n");

        int end_tm = gettimeofday(&end_time, NULL);
        if (end_tm == -1)
            fprintf(stderr, "Error in getting end time\n");

        float real_time = time_parser2(start_time, end_time);

        if (WIFEXITED(status) != 0) {
            exit_status = WEXITSTATUS(status);
            if (exit_status != 0) fprintf(stderr, "Child process %d exited with return value %d\n", pid, exit_status);
            else fprintf(stderr, "Child process %d exited normally\n", pid);
        }
        else if (WIFSIGNALED(status)) {
            exit_status = 128 + WTERMSIG(status);
            fprintf(stderr, "Child process %d exited with signal %d (%s)\n",
                    pid, WTERMSIG(status), strsignal(WTERMSIG(status)));
        }
        else{
            fprintf(stderr, "Child process %d exited normally\n", pid);
        }

        fprintf(stderr, "Real: %fs User: %fs Sys: %fs\n",
                real_time, 
                time_parser(usage.ru_utime),
                time_parser(usage.ru_stime));
    }
    return exit_status;
}