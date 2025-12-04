#include "sem.h"

int main() {
    static int my_procnum = 0;
    for (int i =0; i<N_PROC; i++) {
        my_procnum = i;
        int pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        else if (pid == 0) {
            
            break;
        }
        else {

        }


    }

}