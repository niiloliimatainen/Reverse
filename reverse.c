#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024

struct lines {
	char *string;
	struct lines *prev;
	struct lines *next;
};


/*Function to read lines from the input stream to the doubly linked list*/
int read_lines(struct lines **line, FILE *stream) {
	char tmp[MAXLEN];
	struct lines *lp;

	while (fgets(tmp, MAXLEN, stream) != NULL) {

		if (*line == NULL) {
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "ERROR: could not allocate memory\n");
				return -1;
			}
			lp->prev = lp->next = NULL;
		
		} else {

			if ((lp->next = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "ERROR: could not allocate memory\n");
				return -1;
			}
			lp->next->prev = lp;
			lp = lp->next;
			lp->next = NULL;
		}

		if ((lp->string = malloc(strlen(tmp) + 1)) == NULL) {
			fprintf(stderr, "ERROR: could not allocate memory\n");
			return -1;
		}

		strcpy(lp->string, tmp);
	}

	return 0;
}


/*Function to write lines from doubly linked list in reverse order to the output stream*/
void write_lines(struct lines *line, FILE *stream) {
	struct lines *lp;
	lp = line;
	/*Going to the end of the doubly linked list*/
	while (lp->next != NULL) {
		lp = lp->next;
	}
	/*Writing the doubly linked list in reverse order to the output stream*/
	while (lp != NULL) {
		fprintf(stream, "%s", lp->string);
		lp = lp->prev;
	}
}


/*Function to free the allocated doubly linked list*/
void delete_lines(struct lines *line) {
	struct lines *lp;
	lp = line;
	while (lp != NULL) {
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}
}


int main(int argc, char **argv) { 
	/*IF argc = 1 -> read from stdin and write to stdout*/
	/*IF argc = 2 -> user gives inputfile which prints to screen <stdout>*/
	/*IF argc = 3 -> same thing as 2, but writes to provided file instead of stdout*/
	/*IF ELSE be mad and crash*/
	struct lines *line = NULL;

	if (argc == 1) {
		printf("Input your text: \n");
		if (read_lines(&line, stdin) == -1) {
			exit(1);
		}
		printf("\nData from stdin in reversed order:\n");
		write_lines(line, stdout);

	} else if (argc == 2) {
		FILE *input;

		if ((input = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
			exit(1);
		}
		printf("Data from file '%s' in reversed order:\n", argv[1]);
		if (read_lines(&line, input) == -1) {
			exit(1);
		}
		write_lines(line, stdout);
		fclose(input);

	} else if (argc == 3) {
		if (strcmp(argv[1], argv[2]) == 0) {
			fprintf(stderr, "Input and output file must differ\n");
			exit(1);
		}
		FILE *input;
		FILE *output;
		if ((input = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
			exit(1);
		}

		if (read_lines(&line, input) == -1) {
			exit(1);
		}
		if ((output = fopen(argv[2], "w")) == NULL) {
			fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
			exit(1);
		}
		write_lines(line, output);
		printf("Data from file '%s' writed to the file '%s' in reversed order\n", argv[1], argv[2]);
		fclose(input);
		fclose(output);

	} else {
		printf("Wrong amount of arguments!\n Usage: reverse <optional inputfile.txt> <optional outputfile.txt>\n");
		exit(1);
	}

	delete_lines(line);
	return 0;
}
