#include "wordsearch.h"


char* toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
    return str;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "wordsearch: invalid number of arguments\n");
        return 1;
    }
    char* input_file = argv[1];
    FILE* fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "wordsearch: cannot open file %s\n", input_file);
        return 1;
    }
    // here im assuming the length of each word is at most 255 chars, even though from wordgen its 10 chars max
    int numLines = 0;
    while (!feof(fp)) {
        char ch = fgetc(fp);
        if (ch == '\n') {
            numLines++;
        }
    }
    int seek_return = fseek(fp, 0, SEEK_SET);
    if (seek_return != 0) {
        fprintf(stderr, "wordsearch: error resetting file pointer\n");
        fclose(fp);
        return 1;
    }

    char** words = malloc(numLines * sizeof(char*));
    if (words == NULL) {
        fprintf(stderr, "wordsearch: allocation error\n");
        fclose(fp);
        return 1;
    }

    char buffer[256];
    numLines = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        words[numLines++] = strdup(toUpperCase(buffer));
    }

    while (fgets(buffer, sizeof(buffer), stdin)){
        buffer[strcspn(buffer, "\r\n")] = 0;
        for (int i = 0; i < numLines; i++) {
            if (strcmp(toUpperCase(buffer), words[i]) == 0) {
                printf("%s\n", buffer);
            }
        }
    }

    return 0;
}


