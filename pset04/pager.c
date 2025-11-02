#include "pager.h"

int main() {
    FILE* terminal = fopen("/dev/tty", "r");
    
    char buffer[1024];

    int run = 1;
    char* input;
    while (run ) {
        for (int count = 0; count < 23; count++) {
            input = fgets(buffer, sizeof(buffer), stdin);
            if (input == NULL) {
                fprintf(stderr, "Error reading input from terminal");
                exit(1);
            }
            // eof
            if (feof(terminal)) {
                run = 0;
                break;
            }
            printf("%s", buffer);
        }
        printf("---Press RETURN for more---");
        input = fgets(buffer, sizeof(buffer), terminal);
        if (input == NULL) {
            fprintf(stderr, "Error reading input from terminal");
            exit(1);
        }
        if (buffer[0] == 'q' || buffer[0] == 'Q') {
            run = 0;
        }
        else if (buffer[0] == '\n') {
            continue;
        }
    }
    

    return 0;
}