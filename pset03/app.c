#include "myshell.h"
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    FILE *input = stdin; //default input is stdin
    bool interactive = true; //default should be interactive shell

    if(argc > 1){ //interpreter accepts a file
        interactive = false;
        input = fopen(argv[1], "r");
        if (!input){
            fprintf(stderr, "myshell: cannot open script file '%s': %s\n", argv[1], strerror(errno));
            exit(EXIT_FAILURE);
        }

        myshell_loop(input, interactive, argc, argv); //script interpreter
        fclose(input);

    }else{
        myshell_loop(input, interactive, argc, argv); //standard shell 
    }


    return 0;
}