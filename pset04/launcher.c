#include "launcher.h"



int main(int argc, char *argv[]) {
    int lim = 0;
    if (argc == 2) {
        lim = atoi(argv[1]);
    }

    int fd1[2], fd2[2];
    if (pipe(fd1) == -1) {
        perror("pipe 1");
        exit(1);
    }

    if (pipe(fd2) == -1) {
        perror("pipe 2");
        exit(1);
    }

    int pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) { // wordgen
        close(fd1[0]); // close read end
        dup2(fd1[1], STDOUT_FILENO); // redirect stdout to pipe
        close(fd1[1]);

        if (lim) {
            char lim_str[20];
            sprintf(lim_str, "%d", lim);
            execlp("./wordgen", "./wordgen", lim_str, NULL);
        } else {
            execlp("./wordgen", "./wordgen", NULL);
        }
        perror("execlp wordgen");
        exit(1);
    }

    int pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) {
        dup2(fd1[0], STDIN_FILENO); //  stdin from pipe
        close(fd1[0]);
        close(fd1[1]);
        dup2(fd2[1], STDOUT_FILENO); 
        close(fd2[1]);
        execlp("./wordsearch", "./wordsearch", "words.txt", NULL);
        perror("execlp wordsearch");
        exit(1);
    }
    close(fd1[0]);
    close(fd1[1]);
    int pid3 = fork();
    if (pid3 < 0){
        perror("fork");
        exit(1);
    }

    if (pid3 == 0) {
        close(fd2[1]);
        dup2(fd2[0], STDIN_FILENO); //  stdin from pipe
        close(fd2[0]);

        execlp("./pager", "./pager", NULL);
        perror("execlp pafger");
        exit(1);
    }

    close(fd2[0]);
    close(fd2[1]);
    int status1;
    int status2;
    int status3;
    waitpid(pid1, &status1, 0);
    if (WIFEXITED(status1) && WEXITSTATUS(status1) != 0) {
        fprintf(stderr, "wordgen failed\n");
        exit(1);
    }
    else {
        waitpid(pid2, &status2, 0);
        if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0){
            fprintf(stderr, "wordsearch failed\n");
            exit(1);
        }
        else {
            
            waitpid(pid3, &status3, 0);
            if (WIFEXITED(status3) && WEXITSTATUS(status3) != 0) {
                fprintf(stderr, "pager failed\n");
                exit(1);
            }
            else {
                printf("Wordgen child pid %d exited with status %d\n", pid1, WEXITSTATUS(status1));
                printf("Wordsearch child pid %d exited with status %d\n", pid2, WEXITSTATUS(status2));
                printf("Pager child pid %d exited with status %d\n", pid3, WEXITSTATUS(status3));
            }
            }
        }
    return 0;
}
