/*
Zidane Karim -- zidanekarim @ github
*/

#include "mystdio.h"

struct MYSTREAM *myfopen(const char *pathname, const char *mode) {
    int fd;
    if (strcmp("r", mode) == 0) {
        fd = open(pathname, O_RDONLY, 0666); // open with read only mode
        if (fd == -1) {
            return NULL; // not printing errors or setting ERRNO because 1) open will set errno 2) lib should not print error
        }
    }

    else if (strcmp("w", mode) == 0) {
        fd = open(pathname, O_TRUNC | O_WRONLY |O_CREAT, 0666); // open with write only mode as well as truncating existing files/creating new ones
        if (fd == -1) {
            return NULL;
        }
    } 
    else {
        errno = EINVAL;
        return NULL;
    }
    struct MYSTREAM* open_f = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM));
    if (open_f == NULL) {
        return NULL; 
    }
    char* buffer = (char*) malloc(BUFSIZ*sizeof(char)+1); // add 1 for null terminator
    if (buffer == NULL) {
        free(open_f);
        return NULL; 
    }
    open_f->fd = fd;
    open_f->buffer = buffer;
    open_f->mode = mode;
    return open_f;
}



struct MYSTREAM *myfdopen(int filedesc, const char *mode) {
    if (!(strcmp("r", mode) == 0 || strcmp("w", mode) == 0)) {
        errno = EINVAL; // set invalid mode errno and return
        return NULL;
    }

    struct MYSTREAM* open_f = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM)); // MYSTREAM exists but we need an allocation to store it
    if (open_f == NULL) {
        return NULL; 
    }

    char* buffer = (char*) malloc(BUFSIZ*sizeof(char)+1); // add 1 for null terminator
    if (buffer == NULL) {
        free(open_f);
        return NULL; 
    }

    open_f->fd = filedesc; 
    open_f->buffer=buffer;
    open_f->mode=mode; // no need to think about mode too much given the provision, "It is assumed that the file descriptor was opened with a mode that is identical to or compatible with mode."
    return open_f;
}



int myfgetc(struct MYSTREAM *stream);
int myfputc(int c,struct MYSTREAM *stream);
int myfclose(struct MYSTREAM *stream);
