#include "myshell.h"
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    FILE *input = stdin; //default input is stdin
    bool interactive = true; //default should interactive shell

    if(argc == 2){ //interpreter accepts a file
        interactive = false;
        input = fopen(argv[1], "r");
        if (!input) {
            perror("myshell: cannot open script file");
        exit(EXIT_FAILURE);
        }

        myshell_loop(input, interactive);
        fclose(input);

    }else{
        myshell_loop(input, interactive); //standard shell 
    }


    return 0;
}