#include "simplefind.h"



struct entry** search(char* pattern, char* path, bool verbose, bool x) {
    DIR* directory = opendir(path);
    if (directory == NULL) return NULL;

    struct dirent* dir_entry;
    struct entry** results = NULL;
    size_t count = 0;

    // now we use readdir to read contents of directory
    while ((dir_entry = readdir(directory)) != NULL) {
        if (fnmatch(dir_entry->d_name, ".", 0) == 0 || fnmatch(dir_entry->d_name, "..", 0) == 0) {
            continue; 
        }
        if (fnmatch(dir_entry->d_name, pattern, 0))


}
