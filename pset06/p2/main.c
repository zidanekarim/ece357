#include "fifo.h"
#include "sem.h"
#define READERS 1
#define WRITERS 10
int my_procnum; // just for testing

int main()
{
    struct myfifo *f = mmap(NULL, // my prettier keeps being annoying about this block for some reason 
        sizeof(struct myfifo),
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_SHARED,
        -1,
        0);
    if (f == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    fifo_init(f);
    int pid = fork(); // two virtual processes, one writer, one reader
    if (pid == 0)
    {
        my_procnum = 1;
        for (unsigned long i = 0; i < WRITERS; i++)
        {
            fifo_wr(f, i);
            printf("Wrote: %lu\n", i);
        }
    }
    else
    {
        my_procnum = 0;
        for (unsigned long i = 0; i < READERS; i++)
        {
            unsigned long d = fifo_rd(f);
            printf("Read: %lu\n", d);
        }

        wait(NULL);
        if (munmap(f, sizeof(struct myfifo)) == -1)
        {
            perror("munmap failed");
            exit(1);
        }
    }

    return 0;
}