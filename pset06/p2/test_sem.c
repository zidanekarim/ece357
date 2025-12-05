#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include "sem.h"

#define N_ITERS  100000

int my_procnum;   // global for sem.c

int main(void) {
    struct sem *s = mmap(NULL,
                         sizeof(struct sem),
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS,
                         -1, 0);
    if (s == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    sem_init(s, 0);

    // spawn consumers
    for (int i = 0; i < N_PROC; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            my_procnum = i;
            for (int k = 0; k < N_ITERS; k++) {
                sem_wait(s);
            }
            printf("Consumer %d done\n", i);
            exit(0);
        }
    }

    // producer in parent
    for (int k = 0; k < N_PROC * N_ITERS; k++) {
        sem_inc(s);
    }
    printf("Producer done\n");

    for (int i = 0; i < N_PROC; i++) {
        wait(NULL);
    }

    return 0;
}
