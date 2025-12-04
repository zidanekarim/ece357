#include "sem.h"

struct sem s;

int main()
{
    sem_init(&s, 0); // start locked

    for (int i = 0; i < N_PROC; i++)
    {
        int pid = fork();

        if (pid == 0)
        {
            my_procnum = i;
            printf("Child %d waiting...\n", i);
            sem_wait(&s);
            printf("Child %d woke up!\n", i);
            exit(0);
        }
    }

    sleep(1);

    for (int i = 0; i < N_PROC; i++)
    {
        printf("Waking one...\n");
        sem_inc(&s);
        sleep(1);
    }

    for (int i = 0; i < N_PROC; i++)
        wait(NULL);
}
