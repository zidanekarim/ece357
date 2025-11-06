#include "launcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

    if (pid1 == 0) {
        // first child: wordgen
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
        dup2(fd1[0], STDIN_FILENO); // redirect stdin from pipe
        close(fd1[0]);
        dup2(fd2[1], STDOUT_FILENO); // redirect stdout to pipe
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
        
        dup2(fd2[0], STDIN_FILENO); // redirect stdin from pipe
        close(fd2[0]);

        execlp("./pager", "./pager", NULL);
        perror("execlp pafger");
        exit(1);
    }

    // parent

    close(fd2[0]);
    close(fd2[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}
