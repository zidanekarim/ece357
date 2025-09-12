#include "mystdio.h"

struct MYSTREAM *myfopen(const char *pathname, const char *mode) {
    int fd;
    if (strcmp("r", mode) == 0) {
        fd = open(pathname, O_RDONLY); // open with read only mode
        if (fd == -1) {
            return NULL; // not printing errors or setting ERRNO because 1) open will set errno 2) lib should not print error
        }
    }

    else if (strcmp("w", mode) == 0) {
        fd = open(pathname, O_TRUNC | O_WRONLY |O_CREAT); // open with write only mode as well as truncating existing files/creating new ones
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
    char* buffer = (char*) malloc(BUFSIZ * sizeof(char)+1);
    if (buffer == NULL) {
        return NULL; 
    }
    open_f->fd = fd;
    open_f->buffer = buffer;
    open_f->mode = mode;
    return open_f;
}



struct MYSTREAM *myfdopen(int filedesc, const char *mode) {

}



int myfgetc(struct MYSTREAM *stream);
int myfputc(int c,struct MYSTREAM *stream);
int myfclose(struct MYSTREAM *stream);
