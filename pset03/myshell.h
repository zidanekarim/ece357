#ifndef MYSHELL_H
#define MYSHELL_H
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
struct command {
    char **args;
    char* input_file;
    char* output_file;
    char* error_file;
    int append; // 1 if append true
    int error_append; // 1 if append true
};

#define ARG_MAX 2097152 // max length of args in my (zidane's) linux system, found with getconf ARG_MAX
int myshell_loop(void);
int myshell_execute(struct command* cmd);

char* file_parser(char* input_str);




#endif