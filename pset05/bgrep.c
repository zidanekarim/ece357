#include "bgrep.h"



int pattern_match(int fd, const char *binary_pattern, int context) {
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        return -1;
    }
    int file_size = file_stat.st_size;
    char *map = (char *) mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0); /*
    NULL - let the system choose the address
    file_size - size of the mapping
    PROT_READ - read only
    MAP_PRIVATE - changes are private (idk if this matters but I stole it)
    */
    if (map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    int pattern_length = strlen(binary_pattern);
    if (pattern_length == 0 || pattern_length > file_size) {
        munmap(map, file_size);
        return 1; 
    }
    for (int i = 0; i <= file_size - pattern_length; i++) {
        if (fstat(fd, &file_stat) == -1) // check again
        {
            perror("fstat");
            return -1;
        }
        int new_file_size = file_stat.st_size;
        if (new_file_size < file_size) {
            raise(SIGBUS);
            return -1;
        }

        if (memcmp(map + i, binary_pattern, pattern_length) == 0) {
            if (context > 0) {
                int start = i - context;
                if (start < 0) {
                    start = 0;
                }
                int end = i + pattern_length + context;
                if (end > file_size) {
                    end = file_size;
                }
                for (int j = start; j < end; j++) {
                    if (map[j] >= 32 && map[j] <= 126) {
                        printf("%c", map[j]);
                    } else {
                        printf("?");
                    }
                }
                printf("\n");
            }


            if (munmap(map, file_size) == -1) {
                perror("munmap");
                return -1;
            }
            return 0; // Pattern found
        }
    }

    return 1;
    


}