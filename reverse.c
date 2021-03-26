#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024

struct lines
{
	char *string;
	struct lines *prev;
	struct lines *next;
};

int read_lines(struct lines **line, FILE *stream)
{
	char tmp[MAXLEN];
	struct lines *lp;

	while (fgets(tmp, MAXLEN, stream) != NULL)
	{

		if (*line == NULL)
		{
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL)
			{
				fprintf(stderr, "ERROR: could not allocate memory\n");
				return -1;
			}
			lp->prev = lp->next = NULL;
		}
		else
		{
			if ((lp->next = malloc(sizeof(struct lines))) == NULL)
			{
				fprintf(stderr, "ERROR: could not allocate memory\n");
				return -1;
			}

			/* Here you should insert the lines that link the list correctly */
			lp->next->prev = lp;
			lp = lp->next;
		}

		if ((lp->string = malloc(strlen(tmp) + 1)) == NULL)
		{
			fprintf(stderr, "ERROR: could not allocate memory\n");
			return -1;
		}

		strcpy(lp->string, tmp);
	}

	return 0;
}

void print_lines(struct lines *line, FILE *stream)
{
	struct lines *lp;
	/* Here you should implement a loop that prints the lines in reverse order. 	*/
	/* First you need to follow the links from the first node to the last node.	*/
	lp = line;
	printf("Here is the stdin in reverse direction\n");
	/*Muutetaan jokaisen solmun prev ja next osoittimet keskenään*/
	while (lp != NULL)
	{
		line = lp->prev;
		lp->prev = lp->next;
		lp->next = line;
		lp = lp->prev;
	}

	lp = line->prev;
	while (lp != NULL)
	{
		printf("%s", lp->string);
		lp = lp->next;
	}
	printf("\n");
}

void delete_lines(struct lines *line)
{
	struct lines *lp;
	lp = line;
	while (lp != NULL)
	{
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}
}

int main(int argc, char **argv)
{ /*IF argc = 1 read from stdin and write to stdout*/
	/*IF argc = 2 -> user gives inputfile which prints to screen <stdout>*/
	/*IF argc = 3 -> same thing as 2, but writes to provided file instead of stdout*/
	/*IF ELSE be mad and crash*/
	struct lines *line = NULL;

	if (argc == 1)
	{
		if (read_lines(&line, stdin) == -1)
			exit(1);
		print_lines(line, stdout);
	}
	else if (argc == 2)
	{
		FILE *input;

		if ((input = fopen(argv[1], "r")) == NULL)
		{
			printf("Error: cannot open file '%s'", argv[1]);
			exit(1);
		}

		if (read_lines(&line, input) == -1)
		{
			exit(1);
		}
		print_lines(line, stdout);
	}
	else if (argc == 3)
	{
		FILE *input;
		FILE *output;
		if ((input = fopen(argv[1], "r")) == NULL)
		{
			printf("Error: cannot open file '%s'", argv[1]);
			exit(1);
		}

		if (read_lines(&line, input) == -1)
		{
			exit(1);
		}
		if ((output = fopen(argv[2], "w")) == NULL)
		{
			printf("Error: cannot open file '%s'", argv[2]);
			exit(1);
		}

		print_lines(line, output);
	}
	else
	{
		printf("Wrong amount of arguments!\n Usage: reverse <optional inputfile.txt> <optional outputfile.txt>");
		exit(1);
	}

	delete_lines(line);
	return 0;
}
