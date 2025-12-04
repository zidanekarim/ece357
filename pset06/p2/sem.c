#include "sem.h"


void sigusr1_handler(int signum) {
    ;
}


void sem_init(struct sem *s, int count) {
    s->count = count;
    s->lock = 0;  
    for (int i = 0; i < N_PROC; i++) {
        s->waiting[i] = 0;
        s->pids[i] = 0;
    }
}

int sem_try(struct sem *s) { // attempts P-operation but if the semaphore is not available, it returns immediately
    spin_lock(&s->lock); // This check and decrement is atomic
    if (s->count > 0)
    {
        s->count--;
        spin_unlock(&s->lock);
        return 1; // Successfully acquired the semaphore
    }
    else
    {
        spin_unlock(&s->lock);
        return 0; // Semaphore not available
    }
    
}
void sem_wait(struct sem *s) {    // "P-operation"
    // handler for SIGUSR1
    struct sigaction sa = {0};
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    sigset_t block_mask, oldmask, suspend_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);

    while (1)
    {
        spin_lock(&s->lock);
        if (s->count > 0)
        {
            s->count--;
            spin_unlock(&s->lock);
            return;
        }

        s->waiting[my_procnum] = 1;
        s->pids[my_procnum] = getpid();
        spin_unlock(&s->lock);

        // block SIGUSR1, save old mask
        sigprocmask(SIG_BLOCK, &block_mask, &oldmask);

        suspend_mask = oldmask;
        sigdelset(&suspend_mask, SIGUSR1); // clear

        sigsuspend(&suspend_mask);

        sigprocmask(SIG_SETMASK, &oldmask, NULL);
    }
}
void sem_inc(struct sem *s) {
    spin_lock(&s->lock);
    s->count++;
    if (s->count > 0) { // waking up prcocesses
        for (int i = 0; i < N_PROC; i++) {
            if (s->waiting[i] == 1) {
                s->waiting[i] = 0;
                kill(s->pids[i], SIGUSR1); 
            }
        }
    
    }
    spin_unlock(&s->lock);
}
