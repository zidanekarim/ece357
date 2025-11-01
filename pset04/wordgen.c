#include "wordgen.h"


int main(int argc, char **argv) {
    srand(time(NULL));
    char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int lim = 0;
    if (argc == 2) {
        lim = atoi(argv[1]);
        if (lim <= 0) {
            fprintf(stderr, "wordgen: invalid argument %s\n", argv[1]);
            return 1;
        }
    }
    if (lim) {
        for (int i = 0; i < lim; i++) {
            int word_len = (rand() % 8) + 3;
            for (int j = 0; j < word_len; j++) {
                char c = alphabet[rand() % strlen(alphabet)];
                putchar(c);
            }
            putchar('\n');
        }
    } else {
        while (1) {
            int word_len = (rand() % 8) + 3; 
            for (int j = 0; j < word_len; j++) {
                char c = alphabet[rand() % strlen(alphabet)];
                putchar(c);
            }
            putchar('\n');
        }
    }
    
    
    
    return 0;
}


