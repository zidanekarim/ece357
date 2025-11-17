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
            printf("Option -p selected\n");
            pattern_file = argv[optind]; // not sure why optarg doesn't work here?
            break;

        case 'c':
            context = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Unknown option -%c\n", optopt);
            fprintf(stderr, "Usage: %s [OPTIONS] <binary_pattern> <file_path>\n", argv[0]);
            return -1;
        }
    }
    if (optind < argc) {
        files = malloc((argc - optind - 1) * 1024);
    }
    else {
        fprintf(stderr, "No files provided\n");
        return -1;
    }

    for (int i = 0; i < argc - optind - 1; i++)
    {
        files[i] = argv[optind + 1 + i];
        printf("File to search: %s\n", files[i]);
    }
    char *binary_pattern;
    if (pattern_file != NULL)
    {
        FILE *pf = fopen(pattern_file, "r");
        if (pf == NULL) {
            fprintf(stderr, "Error opening pattern file %s: %s\n", pattern_file, strerror(errno));
            return -1;
        }
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), pf) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            binary_pattern = buffer;
        }
        fclose(pf);
    }
    else binary_pattern = argv[optind];
    printf("Searching for pattern: %s\n", binary_pattern);

    for (int i = 0; i < argc - optind -1; i++) {
        const char *file_path = files[i];
        int fd = open(file_path, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Error opening file %s: %s\n", file_path, strerror(errno));
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