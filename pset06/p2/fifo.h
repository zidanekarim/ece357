#ifndef FIFO_H
#define FIFO_H
#define MYFIFO_BUFSIZ 4096

#include "sem.h"

struct myfifo {
    long int* buffer;
    long int* next_rd;
    long int* next_wr;
    struct sem sem_empty;
    struct sem sem_full;
};

void fifo_init(struct myfifo *f);
void fifo_wr(struct myfifo *f,unsigned long d);
unsigned long fifo_rd(struct myfifo *f);

#endif