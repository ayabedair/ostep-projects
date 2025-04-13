#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            printf("wunzip: cannot open file\n");
            return 1;
        }

        int count;
        char c;

        while (fread(&count, sizeof(int), 1, fp) == 1) {
            if (fread(&c, sizeof(char), 1, fp) != 1) {
                printf("wunzip: failed to read file\n");
                fclose(fp);
                return 1;
            }

            for (int j = 0; j < count; j++) {
                putchar(c);
            }
        }

        fclose(fp);
    }

    return 0;
}
