#include "sem.h"

void sem_init(struct sem *s, int count) {
    s->count = count;
    s->lock = 0; 
    if (s->count == 1) {
        s->lock = 1; // When the initial value is 1, the semaphore acts as a mutex lock.
    }
    for (int i = 0; i < MAX_PROCS; i++) {
        s->waiting[i] = 0;
        s->pids[i] = 0;
    }
}
`
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
void sem_wait(struct sem *s) {                        // "P-operation"
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);


    while (1) {


        spin_lock(&s->lock); // This check and decrement is atomic
        if (s->count > 0)
        {
            s->count--;
            spin_unlock(&s->lock);
            return; 
        }
        else {
            s->waiting[my_procnum] = 1; // 
            s->pids[my_procnum] = getpid();
            spin_unlock(&s->lock);
            sigprocmask(SIG_BLOCK, &mask, &oldmask); 
            sigsuspend(&oldmask); 
            sigprocmask(SIG_SETMASK, &mask, NULL);
        }

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
