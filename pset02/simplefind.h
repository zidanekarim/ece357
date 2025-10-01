#ifndef SIMPLEFIND_H
#define SIMPLEFIND_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>


struct entry {
    int inode;
    int blocks;
    mode_t mode;
    int nlinks;
    char* u_owner;
    char* g_owner;
    off_t size_b;
    blksize_t  blksize;  /* Block size for filesystem I/O  (this is from stat man page) */
    blkcnt_t   blocks;   /* Number of 512 B blocks allocated (this is from stat man page) */
    struct timespec  st_mtime;  /* Time of last modification */
    char* pathname;
    char* symlink; 



};

int print_entry(char* pattern, char* path, bool verbose);
int traverse(char* pattern, char* path, bool verbose, bool x);

#endif