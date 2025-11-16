#include "bgrep.h"
#include <getopt.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <binary_pattern> <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char c;
    char** files;
    int context = 0;
    char* pattern_file = NULL;
    while ((c = getopt(argc, argv, "pc:")) != -1){
        switch (c)
        {
        case 'p':

        case 'c':
            break;
        default:
            fprintf(stderr, "Unknown option -%c\n", optopt);
            fprintf(stderr, "Usage: %s [OPTIONS] <binary_pattern> <file_path>\n", argv[0]);
            return -1;
        }
    }
    if (optind < argc) {
        files = malloc((argc - optind) * 1024);
    }
    else {
        fprintf(stderr, "No files provided\n");
        return -1;
    }
    for (int i = optind; i < argc; i++) {
        files[i - optind] = argv[i];
    }

}
    const char *binary_pattern = argv[optind - 1];

    for (int i = 0; i < argc - optind; i++) {
        const char *file_path = files[i];
        int fd = open(file_path, O_RDONLY);
        if (fd == -1) {
            perror("open");
            continue;
        }
        if (pattern_match(fd, binary_pattern) == 0) {
            printf("Pattern found in file: %s\n", file_path);
        } else {
            printf("Pattern not found in file: %s\n", file_path);
        }
        close(fd);
    }
    free(files);
    return EXIT_SUCCESS;
}