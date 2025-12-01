#ifndef SPINLOCK_H
#define SPINLOCK_H
#define N_PROC 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <ctype.h>
#include <glob.h>
#include <time.h>
#include <sys/mman.h>
#include "tas.h"
#include <sched.h>

int spin_lock(volatile int *lock);
int spin_unlock(volatile int *lock);

#endif