#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH  1024
int main(int argc, char **argv) {

	//Check number of arguments
	if (argc < 2) {
		printf("wgrep: searchterm [file ...]\n");
		exit(EXIT_FAILURE);
	}

	size_t size = MAX_LINE_LENGTH;
	if (argc == 2) {
		char* buf = NULL;
		while(getline(&buf, &size, stdin) != EOF)
		{
			if(strstr(buf, argv[1]) != NULL)
			{
				printf("%s", buf);
			}
		}

		return 0;
	}	

	for (int i = 2; i < argc; i++) {
        	FILE *file = fopen(argv[i], "r");
        	if (file == NULL) {
            		printf("wgrep: cannot open file\n");
            		exit(EXIT_FAILURE);
        	}

        	char line[size];
    		while (fgets(line, size, file) != NULL) {
        		if (strstr(line, argv[1]) != NULL) {
            			printf("%s", line);
				if (i > argc - 1)
					printf("\n");
        		}
    		}	

        	fclose(file);
        }

	return 0;
}
