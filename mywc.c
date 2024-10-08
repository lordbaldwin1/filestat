#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void display_help() {
    printf("Usage:\n");
    printf("-h for help\n");
    printf("-c for count and display the number of characters in the input\n");
    printf("-w for count and display the number of words in the input\n");
    printf("-l for count and display the number of lines in the input\n");
    printf("-f [file_name] for the name of the file to be processed\n");
}

int count_characters(FILE *file) {
    int ch;
    int count = 0;
    while ((ch = fgetc(file)) != EOF) {
        count++;
    }
    return count;
}

int count_words(FILE *file) {
    char *token;
    char line[1000];  // Assuming a line won't exceed 1000 characters
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, " \n\t");
        while (token != NULL) {
            count++;
            token = strtok(NULL, " \n\t");
        }
    }
    return count;
}

int count_lines(FILE *file) {
    int ch;
    int count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    int opt;
    char *filename = NULL;
    FILE *file = stdin;
    int lines = 0, words = 0, chars = 0;
    int c_flag = 0, w_flag = 0, l_flag = 0;
    int f =0;

    while ((opt = getopt(argc, argv, "hcwlf:")) != -1) {
        switch (opt) {
            case 'h':
                display_help();
                return 0;
            case 'c':
                c_flag = 1;
                break;
            case 'w':
                w_flag = 1;
                break;
            case 'l':
                l_flag = 1;
                break;
            case 'f':
                filename = optarg;
		f = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-h] [-c] [-w] [-l] [-f file_name]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (filename && (file = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(2);
    }

    chars = count_characters(file);
    rewind(file);
    words = count_words(file);
    rewind(file);
    lines = count_lines(file);

    if (file != stdin) {
        fclose(file);
    }

    if (!c_flag && !w_flag && !l_flag) {  // If no options, display all counts
        c_flag = w_flag = l_flag = 1;
    }
    if (l_flag) {
        printf("%d ", lines);
    }
    if (w_flag) {
        printf("%d ", words);
    }
    if (c_flag) {
        printf("%d ", chars);
    }
    if(f) {
	    printf("%s", filename);
    }
    return 0;
}
