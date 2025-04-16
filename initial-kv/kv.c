#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SIZE 1024

void put_value(char *command) {
	// Skip 'p,'
	char *data = command + 2;

	// open file in append mode
	FILE *file = fopen("./database.txt", "a");
	if (file == NULL) {
		fprintf(stderr, "Failed to open file");
		return;
	}

	// Add the key-value
	fwrite(data, sizeof(char), strlen(data), file);
	fwrite("\n", sizeof(char), 1, file); // add new-line
	
	fclose(file);
}

void get_value(char *command) {
    // Skip 'g,'
    char *data = command + 2;

    int search_key;
    char *search_value = NULL;

    // Parse key from command
    char *token = strtok(data, ",");
    if (token == NULL) {
        fprintf(stderr, "Wrong format: expected 'g,key,value'\n");
        return;
    }

    // Convert key to integer
    char *endptr;
    search_key = strtol(token, &endptr, 10);
    if (endptr == token || *endptr != '\0') {
        fprintf(stderr, "Invalid key: must be a number\n");
        return;
    }

    // Open database file
    FILE *file = fopen("./database.txt", "r");
    if (file == NULL) {
        perror("Failed to open database file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        // Remove newline
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        char *line_copy = strdup(line);
        if (!line_copy) {
            perror("Memory allocation failed");
            break;
        }

        char *db_key_str = strtok(line_copy, ",");
        if (db_key_str == NULL) {
            free(line_copy);
            continue;
        }

        int db_key = strtol(db_key_str, &endptr, 10);
        if (endptr == db_key_str || *endptr != '\0') {
            free(line_copy);
            continue;
        }

        // Check if keys match
        if (db_key == search_key) {
            printf("%s\n", line);
            free(line_copy);
            fclose(file);
            free(line);
            return;
        }

        free(line_copy);
    }

    printf("%d not found\n", search_key);
    fclose(file);
    free(line);
}

void delete_value(char *command) {
    // Skip 'd,' prefix
    char *key_str = command + 2;
    if (*key_str == '\0') {
        fprintf(stderr, "Error: Missing key after 'd,'\n");
        return;
    }

    // Convert key to integer
    char *endptr;
    long key = strtol(key_str, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid key format (must be integer)\n");
        return;
    }

    // Open original file for reading
    FILE *src_file = fopen("./database.txt", "r");
    if (!src_file) {
        perror("Error opening database file");
        return;
    }

    // Create temp file
    FILE *tmp_file = fopen("./database.tmp", "w");
    if (!tmp_file) {
        perror("Error creating temporary file");
        fclose(src_file);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int found = 0;

    while ((read = getline(&line, &len, src_file)) != -1) {
        // Remove newline
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        char *line_copy = strdup(line);
        if (!line_copy) {
            perror("Memory allocation failed");
            break;
        }

        char *db_key_str = strtok(line_copy, ",");
        long db_key = -1;
        if (db_key_str) {
            db_key = strtol(db_key_str, &endptr, 10);
        }

        // If keys don't match copy the line
        if (!db_key_str || *endptr != '\0' || db_key != key) {
            fprintf(tmp_file, "%s\n", line);
        } else {
            found = 1;
        }

        free(line_copy);
    }

    free(line);
    fclose(src_file);
    fclose(tmp_file);

    // Handle result
    if (!found) {
        printf("Key %ld not found in database\n", key);
        remove("./database.tmp");
    } else {
        if (rename("./database.tmp", "./database.txt") != 0) {
            perror("Error updating database file");
            remove("./database.tmp");
        } else {
            printf("Successfully deleted key %ld\n", key);
        }
    }
}

void clear_all() {
    FILE *file = fopen("./database.txt", "w");
    if (file == NULL) {
        perror("Error clearing database");
        return;
    }
    fclose(file);
}

void print_all() {
    FILE *file = fopen("./database.txt", "r");
    if (file == NULL) {
        perror("Error opening database");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        // Remove newline if present
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        printf("%s\n", line);
    }

    free(line);
    fclose(file);
}

