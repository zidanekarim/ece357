#include "fifo.h"
#include "sem.h"

int my_procnum; // just for testing

int main() {
    int data = mmap(NULL, sizeof(struct myfifo), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    struct myfifo *f = (struct myfifo *)data;

    fifo_init(f);
    int pid = fork(); // two virtual processes, one writer, one reader
    if (pid == 0) {
        my_procnum = 1;
        for (unsigned long i = 0; i < 10000; i++) {
            fifo_wr(f, i);
            printf("Wrote: %lu\n", i);
        }
    } else {
        my_procnum = 0;
        for (unsigned long i = 0; i < 10000; i++) {
            unsigned long d = fifo_rd(f);
            printf("Read: %lu\n", d);
        }

        wait(NULL);
        if (munmap(f, sizeof(struct myfifo)) == -1) {
            perror("munmap failed");
            exit(1);
        }
    }

    return 0;


}