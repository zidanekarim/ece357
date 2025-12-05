#include "fifo.h"
#include "sem.h"
#define READERS 10
#define WRITERS 10
#define ITERATIONS 50000
int my_procnum; // just for testing


int check_data(unsigned long data, int writer_id, unsigned long iteration) {
    unsigned long expected = writer_id * ITERATIONS + iteration;
    if (data != expected) {
        printf("expected %lu but got %lu from writer %d at iteration %lu\n", expected, data, writer_id, iteration);
        return 0; // mismatch
    }
    return 1; // match
}

int main()
{
    struct myfifo *f = mmap(NULL,
                            sizeof(struct myfifo), // my prettier keeps messing this block up 
                            PROT_READ | PROT_WRITE,
                            MAP_ANONYMOUS | MAP_SHARED,
                            -1,
                            0);
    if (f == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }
    fifo_init(f);

    for (int writer = 0; writer < WRITERS; writer++)
    {
        int writer_pid = fork();
        if (writer_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        if (writer_pid == 0)
        { // child
            my_procnum = writer;
            for (unsigned long i = 0; i < ITERATIONS; i++)
            {
                fifo_wr(f, (unsigned long)(writer * ITERATIONS + i));
            }
            printf("Writer %d done.\n", writer);
            exit(0);
        }
    }

    for (int reader = 0; reader < READERS; reader++)
    {
        int reader_pid = fork();
        if (reader_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        if (reader_pid == 0)
        { // child
            my_procnum = WRITERS + reader;
            for (unsigned long i = 0; i < (WRITERS * ITERATIONS) / READERS; i++) // formula comes from total writes / total reads
            {
                unsigned long data = fifo_rd(f);
                check_data(data, data / ITERATIONS, data % ITERATIONS);
            }
            printf("Reader %d done.\n", reader);
            exit(0);
        }
    }

    // wait for all writers + readers
    for (int i = 0; i < WRITERS + READERS; i++)
    {
        wait(NULL); // without this, random processes become zombies and started hanging 
    }

    return 0;
}
