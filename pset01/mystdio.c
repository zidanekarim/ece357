/*
Zidane Karim -- zidanekarim @ github
*/

#include "mystdio.h"

struct MYSTREAM *myfopen(const char *pathname, const char *mode, int bufsiz) {
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
    char* buffer = (char*) malloc(bufsiz+1); // add 1 for null terminator
    if (buffer == NULL) {
        free(open_f);
        return NULL; 
    }
    open_f->fd = fd;
    open_f->buffer = buffer;
    open_f->mode = mode;
    open_f->pos=0;
    open_f->readbytes=0;
    open_f->bufsiz=bufsiz;
    return open_f;
}



struct MYSTREAM *myfdopen(int filedesc, const char *mode, int bufsiz) {
    if (!(strcmp("r", mode) == 0 || strcmp("w", mode) == 0)) {
        errno = EINVAL; // set invalid mode errno and return
        return NULL;
    }

    struct MYSTREAM* open_f = (struct MYSTREAM*)malloc(sizeof(struct MYSTREAM)); // MYSTREAM exists but we need an allocation to store it
    if (open_f == NULL) {
        return NULL; 
    }

    char* buffer = (char*) malloc(bufsiz+1); // add 1 for null terminator
    if (buffer == NULL) {
        free(open_f);
        return NULL; 
    }

    open_f->fd = filedesc; 
    open_f->buffer=buffer;
    open_f->mode=mode; // no need to think about mode too much given the provision, "It is assumed that the file descriptor was opened with a mode that is identical to or compatible with mode."
    open_f->pos=0;
    open_f->readbytes=0;
    open_f->bufsiz=bufsiz;
    return open_f;
}



int myfgetc(struct MYSTREAM *stream) {
    if (strcmp(stream->mode, "r") == 0) {
        if (stream->pos >= stream->readbytes) {
            stream->pos=0; // reset pos
            int read_size = read(stream->fd, stream->buffer, stream->bufsiz);
            if (read_size == -1) return -1; 
            else if (read_size==0) {
                errno = 0; // no error, just end of file 
                return -1; 
            }
            stream->readbytes = read_size;

        }
        int next = (int) (stream->buffer)[stream->pos++]; // return int to avoid  EOF char issues. increment pos rather than the buffer itself to preserve data (in case of lseek)

        return next;

    }
    else {
        errno = EBADF;
        return -1;
    }
}


int myfputc(int c,struct MYSTREAM *stream) {
    if (strcmp(stream->mode, "w") == 0) {
        stream->buffer[stream->pos++] = (char) c;
        if (stream->pos==stream->bufsiz) {
            int write_f = write(stream->fd, stream->buffer, stream->pos);
            if (write_f == -1) return -1;

            if (write_f != stream->pos) {
                errno = EIO; // partial write so treat as error
                return -1;
            }
        stream->pos = 0; // reset pos
        }
    }
    else {
        errno = EBADF;
        return -1;
    }
    return c; 
}

int myfclose(struct MYSTREAM *stream) {
    if (strcmp(stream->mode, "w") == 0 && stream->pos > 0) {
        int write_f = write(stream->fd, stream->buffer, stream->pos);
        if (write_f == -1 || write_f != stream->pos) return -1;
    }
    int close_f = close(stream->fd);
    if (close_f == -1) return -1; 
    free(stream->buffer);
    free(stream);
    return 0;
}
