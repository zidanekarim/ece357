#include "simplefind.h"
#include <stdio.h>


int main(int argc, char** argv) {
    // getopt
    bool verbose = false;
    bool x = false;
    char* pattern = NULL;
    char* path = ".";
    char c;
    while ((c = getopt (argc, argv, "l:x:n:")) != -1) {
        switch (c) {
            case 'l':
                verbose = true;
                break;
            case 'x':
                x = true;
                break;
            case 'n':
                pattern = optarg;
                break;
            default:
                fprintf(stdout, "No additional arguments specified\n");
                break;
        }
    }
    if (optind < argc) { // this means there is a path argument
        path = argv[optind];
    }

    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
        fprintf(stderr, "Can't access path %s\n", path);
        return -1;
    }
    dev_t start_dev = path_stat.st_dev;
    int result = traverse(pattern, path, verbose, x, start_dev);
    if (result == -1) {
        fprintf(stderr, "Can't traverse path %s\n", path);
        return -1;
    }

    return 0;
}
