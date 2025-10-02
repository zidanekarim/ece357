#ifndef SIMPLEFIND_H
#define SIMPLEFIND_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>


int print_entry(char* pattern, char* path, bool verbose);
int traverse(char* pattern, char* path, bool verbose, bool x);

#endif