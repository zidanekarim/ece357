#include "mystdio.h"
#include <stdio.h>

int main(int argc, char** argv ) {
    int fd_out;
    int fd_in;
    if (argc>0) { // output or input (at least one) specified
        if (strcmp(argv[1], "-o") == 0) {
            if (argc==4) {
                fd_out = myfopen(argv[2], "w");
                if (fd_out == -1) {
                    perror("Error opening output file\n");
                    return 255;
                }
                
                fd_in = myfopen(argv[3], "r");
                if (fd_out == -1) {
                    perror("Error opening input file\n");
                    return 255;
                }
            }
            else if (argc==3) {
                fd_out = myfopen(argv[2], "w");
                if (fd_out == -1) {
                    perror("Error opening output file\n");
                    return 255;
                }
                
                fd_in = myfdopen(STDIN_FILENO, "r");
                if (fd_out == -1) {
                    perror("Error opening stdin for reading\n");
                    return 255;
                }
            }
            else {
                fprintf(stderr, "Invalid number of arguments specified. Use the -o flag to specifiy an output file e.g. tabstop -o outfile");
                return 255;
            }

        }
        else if (argc > 2) { // example input to trigger this: tabstop outfile infile
            fprintf(stderr, "Invalid number of arguments specified. Use the -o flag to specifiy an output file e.g. tabstop -o outfile infile");
            return 255;
        }
        else if (argc == 2) {
            fd_out = myfdopen(STDOUT_FILENO, "w");
            if (fd_out == -1) {
                perror("Error opening stdout for writing\n");
                return 255;
            }
            
            fd_in = myfopen(argv[1], "r");
            if (fd_out == -1) {
                perror("Error opening input file\n");
                return 255;
            }
        } 

    }
    else {
        fd_out = myfdopen(STDOUT_FILENO, "w");
        if (fd_out == -1) {
            perror("Error opening stdout for writing\n");
            return 255;
        } 
        fd_in = myfdopen(STDIN_FILENO, "r");
        if (fd_out == -1) {
            perror("Error opening stdin for reading\n");
            return 255;
        }
    } // at this point, fd_in and fd_out should both be set to correct files

    // tab parser logic, knowing tab ASCII code == 9

    
    return 0;
}