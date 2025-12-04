#include "fifo.h"

void fifo_init(struct myfifo *f) {
    f->next_rd = f->buffer;
    f->next_wr = f->buffer; // these both start at 0th anyway >_< :P
    sem_init(&f->sem_empty, MYFIFO_BUFSIZ); /// everything empty
    sem_init(&f->sem_full, 0); // nothing full
    sem_init(&f->mutex, 1); // 

}

void fifo_wr(struct myfifo *f,unsigned long d) {
    sem_wait(&f->sem_empty); // wait for empty slot
    sem_wait(&f->mutex); // enter critical section

    *(f->next_wr) = d;
    f->next_wr++;
    if (f->next_wr == f->buffer + MYFIFO_BUFSIZ) {
        f->next_wr = f->buffer; // wrap around since u wanted circular buffer
    }

    sem_unlock(&f->mutex); // leave critical section
    sem_inc(&f->sem_full); // signal that there's a new full slot, waking a reader if needed
}

unsigned long fifo_rd(struct myfifo *f) {
    sem_wait(&f->sem_full); // wait for full slot
    sem_wait(&f->mutex); // enter critical section

    unsigned long d = *(f->next_rd);
    f->next_rd++;
    if (f->next_rd == f->buffer + MYFIFO_BUFSIZ) {
        f->next_rd = f->buffer; // wrap around since u wanted circular buffer
    }

    sem_unlock(&f->mutex); // leave critical section
    sem_inc(&f->sem_empty); // signal that there's a new empty slot, waking a writer if needed

    return d;
}