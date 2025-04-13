#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    int prev_char = EOF;
    int count = 0;
    int current_char;

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: failed to open file\n");
            return 1;
        }

        while ((current_char = getc(fp)) != EOF) {
            if (current_char == prev_char) {
                count++;
            } else {
                if (count > 0) {
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&prev_char, sizeof(char), 1, stdout);
                }
                prev_char = current_char;
                count = 1;
            }
        }
        fclose(fp);
    }

    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev_char, sizeof(char), 1, stdout);
    }

    return 0;
}
