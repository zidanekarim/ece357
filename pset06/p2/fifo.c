#include "fifo.h"

void fifo_init(struct myfifo *f) {
    f->next_rd = f->buffer;
    f->next_wr = f->buffer;
    f->buffer = (long int*) malloc(MYFIFO_BUFSIZ * sizeof(long int));
    sem_init(&f->sem_empty, MYFIFO_BUFSIZ);
    sem_init(&f->sem_full, 0);
}

