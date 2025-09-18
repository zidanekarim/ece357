/*
Zidane Karim -- zidanekarim @ github
*/

#ifndef MYSTDIO_H
#define MYSTDIO_H

#define BUFSIZ 4096 // defined by default in stdio but that lib is not included for now

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct MYSTREAM {
    int fd; //  file descriptor is lowest value from C open syscall
    char* mode;  // file open mode
    char* buffer;
    int pos;
    int readbytes;
    int bufsiz;
    //int writebytes;
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode, int bufsiz);
struct MYSTREAM *myfdopen(int filedesc, const char *mode, int bufsiz);
int myfgetc(struct MYSTREAM *stream);
int myfputc(int c,struct MYSTREAM *stream);
int myfclose(struct MYSTREAM *stream);

#endif