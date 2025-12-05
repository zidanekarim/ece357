#include "fifo.h"
#include "sem.h"

#define READERS   1
#define WRITERS   30
#define ITERATIONS 10000

int my_procnum; 

static long last_seen[WRITERS];

static void init_last_seen(void) {
    for (int i = 0; i < WRITERS; i++) {
        last_seen[i] = -1;   // none seen yet
    }
}


static void check_data(unsigned long data) {
    int writer_id = data / ITERATIONS;
    unsigned long seq = data % ITERATIONS;

    long prev = last_seen[writer_id];

    if (prev == -1) {
        // first word for this writer
        if (seq != 0) {
            printf("detected corruption for writer %d expected seq=0 got %lu (data=%lu)\n",
                   writer_id, seq, data);
        }
    } else {
        long expected = prev + 1;
        if (seq != (unsigned long)expected) {
            printf("detected corruption for writer %d expected seq=%ld got %lu (data=%lu)\n",
                   writer_id, expected, seq, data);
        }
    }

    last_seen[writer_id] = (long)seq;
}

int main(void)
{
    // shared FIFO in MAP_SHARED memory
    struct myfifo *f = mmap(NULL,
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

    init_last_seen();

    // Spawn writers
    for (int writer = 0; writer < WRITERS; writer++) {
        pid_t writer_pid = fork();
        if (writer_pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (writer_pid == 0) {
            // child: writer process
            my_procnum = writer;
            for (unsigned long i = 0; i < ITERATIONS; i++) {
                unsigned long value = (unsigned long)writer * ITERATIONS + i;
                fifo_wr(f, value);
            }
            printf("Writer %d done.\n", writer);
            exit(0);
        }
    }

    // Spawn reader(s) – spec only *needs* 1, but we keep the loop
    for (int reader = 0; reader < READERS; reader++) {
        pid_t reader_pid = fork();
        if (reader_pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (reader_pid == 0) {
            // child: reader process
            my_procnum = WRITERS + reader;

            unsigned long total = (unsigned long)WRITERS * ITERATIONS / READERS;

            for (unsigned long i = 0; i < total; i++) {
                unsigned long data = fifo_rd(f);
                check_data(data);
            }

            printf("Reader %d done.\n", reader);
            exit(0);
        }
    }

    // Parent waits for all writers + readers
    for (int i = 0; i < WRITERS + READERS; i++) {
        wait(NULL);
    }

    return 0;
}
