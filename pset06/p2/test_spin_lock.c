#include "spinlock.h"

int main() {
    int* data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    data[0] = 0; 
    data[1] = 0; 
    data[2] = 0; 
    for (int i =0; i<N_PROC; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            for (int j = 0; j < 6767676; j++) {
                data[2] += 1; // not working correctly without lock
                spin_lock(&data[0]);
                data[1] += 1; // crit works 
                spin_unlock(&data[0]);                
            }
            exit(0);
        }
    }
    printf("parent\n");
    for (int i = 0; i < N_PROC; i++) {
        wait(NULL);
    }
    printf("Final value with spinlock: %d (should be %d)\n", data[1], N_PROC * 6767676);
    printf("Final value without spinlock: %d (should be %d)\n", data[2], N_PROC * 6767676);
    munmap(data, 4096);
    return 0;

    
}