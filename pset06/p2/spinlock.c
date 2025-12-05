#include "spinlock.h"

int spin_lock(volatile int *lock) {
    while (tas((volatile char*) lock)) {
        sched_yield();
    }
    return 0; 
}

int spin_unlock(volatile int *lock) {
    *lock = 0; 
    return 0;
}