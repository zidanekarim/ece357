#include "mystdio.h"
#include <stdio.h>

int main(int argc, char** argv ) {
    struct MYSTREAM* f_out;
    struct MYSTREAM* f_in;
    if (argc>1) { // output or input (at least one) specified
        if (strcmp(argv[1], "-o") == 0) {
            if (argc==4) {
                f_out = myfopen(argv[2], "w");
                if (f_out == NULL) {
                    perror("Error opening output file");
                    return 255;
                }
                
                f_in = myfopen(argv[3], "r");
                if (f_in == NULL) {
                    perror("Error opening input file");
                    return 255;
                }
            }
            else if (argc==3) {
                f_out = myfopen(argv[2], "w");
                if (f_out == NULL) {
                    perror("Error opening output file");
                    return 255;
                }
                
                f_in = myfdopen(STDIN_FILENO, "r");
                if (f_in == NULL) {
                    perror("Error opening stdin for reading");
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
            f_out = myfdopen(STDOUT_FILENO, "w");
            if (f_out == NULL) {
                perror("Error opening stdout for writing");
                return 255;
            }
            
            f_in = myfopen(argv[1], "r");
            if (f_in == NULL) {
                perror("Error opening input file");
                return 255;
            }
        } 

    }
    else {
        f_out = myfdopen(STDOUT_FILENO, "w");
        if (f_out == NULL) {
            perror("Error opening stdout for writing");
            return 255;
        } 
        f_in = myfdopen(STDIN_FILENO, "r");
        if (f_in == NULL) {
            perror("Error opening stdin for reading");
            return 255;
        }
    } // at this point, f_in and f_out should both be set to correct files

    // tab parser logic, knowing tab ASCII code == 9
    int read_char;
    int write_char;
    while ((read_char = myfgetc(f_in)) != -1) {
        if (read_char == 9) {
            for (int i=0; i<4; i++) {
                write_char = myfputc(32, f_out); // 32 is space, writes 4 times then moves to next char 
                if (write_char < 0) {
                    perror("Bad input to output file. Could not write space char");
                    return 255;
                } 
            }
        }
        else if (read_char < 0) {
            perror("Bad read of input file"); 
            return 255;
        }
        else {
            write_char = myfputc(read_char, f_out);
            if (write_char < 0) {
                perror("Bad input to output file. Could not write char");
                return 255;
            } 
        }
    }

    int close_in = myfclose(f_in); 
    if (close_in ==-1) {
        perror("Could not close input");
    }
    int close_out = myfclose(f_out); 
    if (close_out ==-1) {
        perror("Could not close input");
    }

    return 0;
}