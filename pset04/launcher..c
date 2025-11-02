#include "launcher.h"

int main(int argc, char *argv[]){
    int lim;
    if (argc == 2) {
        lim = atoi(argv[1]);
    } 

    int fd[2];
    int pipe_one = pipe(fd);
    if (pipe_one == -1) {
        perror("initial pipe failed");
        exit(1);
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork");
    }
    char buffer[4096];
    if (pid == 0) { // child process is wordgen    
        dup2(fd[1], STDOUT_FILENO); // redirect stdout to pipe write end
        close(fd[0]); // close unused read end
        close(fd[1]); // close original write end
        if (lim) {          
            char lim_str[20];
            sprintf(lim_str, "%d", lim);
            execlp("./wordgen", "./wordgen", lim_str, NULL); 
        }
        else execlp("./wordgen", "./wordgen", NULL);

    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }


    return 0;
}