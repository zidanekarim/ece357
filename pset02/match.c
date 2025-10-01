#include <stdio.h>
#include <fnmatch.h>

int main() {
    char pattern2[] = "simplefind.c";
    char string3[] = "/home/zidane.karim/Documents/ece357/pset02/simplefind.c";

    if (fnmatch(pattern2, string3, 0) == 0) {
        printf("'%s' matches '%s' (with FNM_PATHNAME)\n", pattern2, string3);
    } else {
        printf("'%s' does not match '%s' (with FNM_PATHNAME)\n", pattern2, string3);
    }

    return 0;
    
}