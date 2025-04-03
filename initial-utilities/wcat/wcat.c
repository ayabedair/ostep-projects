#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE  1024
int main (int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
        	int fd = open(argv[i], O_RDONLY);
        	if (fd == -1) {
            		printf("wcat: cannot open file\n");
            		exit(EXIT_FAILURE);
        	}

    // Read and print files contents
		char buf[BUF_SIZE];
        	int bytesRead;

		while ((bytesRead = read(fd, buf, sizeof(buf))) > 0) {
            		if (write(STDOUT_FILENO, buf, bytesRead) == -1) {
                		perror("wcat: failed to write files contents");
                		close(fd);
                		exit(EXIT_FAILURE);
            		}
        	}	

        	if (bytesRead == -1) {
            		perror("wcat: failed to read files contents");
        	}

        	close(fd);
        }

	return 0;
}
