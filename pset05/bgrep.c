#include "bgrep.h"

int pattern_match(int fd, const char *binary_pattern) {
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
    // Search for the binary pattern in the mapped file
    int pattern_length = strlen(binary_pattern);
    for (int i = 0; i <= file_size - pattern_length; i += pattern_length) {
        if (memcmp(map + i, binary_pattern, pattern_length) == 0) {
            munmap(map, file_size);
            return 0; // Pattern found
        }
    }

    return -1;
    


}