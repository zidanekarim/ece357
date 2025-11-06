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
                run = 0;
                break;
            }
            // eof
            if (feof(stdin)) {
                run = 0;
                break;
            }
            printf("%s", buffer);
        }
        printf("---Press RETURN for more---");
        input = fgets(buffer, sizeof(buffer), terminal);
        if (buffer[0] == 'q' || buffer[0] == 'Q') {
            run = 0;
            // sending signal to wordgen to terminate
            fclose(terminal);
            kill(0, SIGPIPE);
            break;
        }
        else if (buffer[0] == '\n') {
            continue;
        }
        else if (feof(terminal)) {
            run = 0;
        }
    }
    

    return 0;
}