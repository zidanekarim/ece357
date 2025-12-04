#ifndef SEM_H
#define SEM_H

#include "spinlock.h"


extern int my_procnum;

struct sem {
    int count;
    volatile int lock;
    char waiting[N_PROC]; // im doing char 0 as non-waiting, 1 as waiting
    int pids[N_PROC];
};

void sem_init(struct sem *s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);



#endif