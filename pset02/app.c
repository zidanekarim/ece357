#include "simplefind.h"
#include <stdio.h>
#include <getopt.h>

/* 
Note. There is noticeably less error printing in this assignment compared to a usual assignment. This is because, aside from the initial path argument, errors are not considered fatal. For example, if a directory cannot be opened due to permissions, we simply skip it and continue. 
This is similar to the behavior of the real 'find' command.
*/

int main(int argc, char** argv) {
    // getopt
    bool verbose = false;
    bool x = false;
    char* pattern = NULL;
    char* path = ".";
    char c;
    while ((c = getopt (argc, argv, "lxn:")) != -1) {
        switch (c) {
            case 'l':
                verbose = true;
                break;
            case 'x':
                x = true;
                break;
            case 'n':
                pattern = optarg;
                if (pattern == NULL || strlen(pattern) == 0) {
                    fprintf(stderr, "Option -n requires an argument\n");
                    fprintf(stderr, "Usage: %s [-l] [-x] [-n pattern] [path]\n", argv[0]);
                    return -1;
                }
                break;
            default:
                fprintf(stderr, "Unknown option -%c\n", optopt);
                fprintf(stderr, "Usage: %s [-l] [-x] [-n pattern] [path]\n", argv[0]);
                return -1;
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
