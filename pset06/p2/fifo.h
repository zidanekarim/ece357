#ifndef FIFO_H
#define FIFO_H
#define MYFIFO_BUFSIZ 4096

#include "sem.h"

struct myfifo {
    unsigned long buffer[MYFIFO_BUFSIZ];
    unsigned long * next_rd;
    unsigned long * next_wr;
    struct sem sem_empty; // writers wait
    struct sem sem_full; // readers wait
    struct sem mutex; // crit section
};

void fifo_init(struct myfifo *f);
void fifo_wr(struct myfifo *f,unsigned long d);
unsigned long fifo_rd(struct myfifo *f);

#endif