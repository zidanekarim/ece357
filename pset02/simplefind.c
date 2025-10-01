#include "simplefind.h"
#include <dirent.h>

int print_entry(char* pattern, char* pathname, bool verbose) {
    if (pattern == NULL) {
        struct stat path_stat;
        if (stat(pathname, &path_stat) == -1) {
            return -1;
        }
        if (verbose) {
            printf("%d  %d %s %d %s %s %d ");
        }
        else {
            printf("%s\n", pathname);
        }

    }
    else {
        // iterate through directory entries and match against pattern

    }

}


int traverse(char* pattern, char* path, bool verbose, bool x) {
    DIR* directory = opendir(path);
    if (directory == NULL) {
        closedir(path);
        return -1;
    }
    struct dirent* dir_entry;
    //struct entry* current;
    //struct entry** results = NULL;
    if (x==true) { // if xdev true, we need to check device ID of current directory
        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            closedir(directory);
            return -1;
        }
        dev_t original_dev = path_stat.st_dev;
        if (original_dev != path_stat.st_dev) {
            closedir(directory);
            return 0; // if device IDs don't match, we skip this directory
        }

    }


    // now we use readdir to read contents of directory
    while ((dir_entry = readdir(directory)) != NULL) { // once directory is empty (meaning we've looked through it, we can return from the function
        char new_path[256];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, dir_entry->d_name);
        if (fnmatch(dir_entry->d_name, ".", 0) == 0 || fnmatch(dir_entry->d_name, "..", 0) == 0) {
            continue; 
        }
        else if (dir_entry->d_type==DT_DIR) {
            // recursively call traverse on this directory
            int result = traverse(pattern, new_path, verbose, x);
            if (result == -1) {
                return -1;
            }
            
            int print_result = print_entry(pattern, new_path, verbose);
            if (print_result == -1) {
                return -1;
            } 
        }
        else { // base case is file or symlink
            int print_result = print_entry(pattern, new_path, verbose);
            if (print_result == -1) {
                return -1;
            }
        }
        return 0;
    }
}
