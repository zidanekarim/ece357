#ifndef MYSTDIO_H
#define MYSTDIO_H

#define BUFSIZ 1024

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct MYSTREAM {
    int fd; //  file descriptor is lowest value from C open syscall
    char* mode;  // file open mode
    char* buffer;
};

struct MYSTREAM *myfopen(const char *pathname, const char *mode);
struct MYSTREAM *myfdopen(int filedesc, const char *mode);
int myfgetc(struct MYSTREAM *stream);
int myfputc(int c,struct MYSTREAM *stream);
int myfclose(struct MYSTREAM *stream);

#endif