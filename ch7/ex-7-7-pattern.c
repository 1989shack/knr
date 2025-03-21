/*
 * Exercise 7-7.
 *
 * Modify the pattern finding program of Chapter 5 to take its input from a set
 * of named files or, if no files are named as arguments, from the standard
 * input.  Should the file name be printed when a matching line is found?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

enum errors {
	NO_ERROR = 0,
	ILLEGAL_OPTION,
	NO_PATTERN,
	FILE_OPEN_ERROR,
	FILE_READ_ERROR
};

struct options {
	int except;
	int filename;
	int number;
	char *pattern;
	int pathargs;
};

static void parseargs(int argc, char *argv[], struct options *);
static int find(FILE *file, char *path, struct options *);
static FILE *chkfopen(char *file, char *modes);
static char *chkfgets(char *s, int n, FILE *iop, char *path);

static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	struct options options;
	int i, found = 0;
	char *path;
	FILE *file;

	parseargs(argc, argv, &options);

	if (options.pathargs < argc)
		for (i = options.pathargs; i < argc; i++) {
			path = argv[i];
			file = chkfopen(path, "r");
			found += find(file, path, &options);
			fclose(file);
		}
	else
		found = find(stdin, "<stdin>", &options);

	exit(NO_ERROR);
}

int find(FILE *file, char *path, struct options *opts)
{
	char line[MAXLINE];
	long lineno = 0, found = 0;

	while ((chkfgets(line, MAXLINE, file, path)) != NULL) {
		lineno++;
		if ((strstr(line, opts->pattern) != NULL) != opts->except) {
			if (opts->filename)
				printf("%s:", path);
			if (opts->number)
				printf("%ld:", lineno);
			printf("%s", line);
			found++;
		}
	}
	return found;
}

void parseargs(int argc, char *argv[], struct options *options)
{
	char c, *arg;
	int ai;

	options->except = options->number = options->filename = 0;

	for (ai = 1; ai < argc && (arg = argv[ai])[0] == '-'; ai++)
		while ((c = *++arg))
			switch (c) {
			case 'x':
				options->except = 1;
				break;
			case 'n':
				options->number = 1;
				break;
			case 'f':
				options->filename = 1;
				break;
			default:
				fprintf(stderr, "find: illegal option %c", c);
				exit(ILLEGAL_OPTION);
			}
	if (ai == argc) {
		fprintf(stderr, "Usage: find -x -n -f pattern\n");
		exit(NO_PATTERN);
	} else {
		options->pattern = argv[ai++];
		options->pathargs = ai;
	}
}

FILE *chkfopen(char *path, char *modes)
{
	FILE *fp = fopen(path, modes);
	if (fp != NULL)
		return fp;
	sprintf(errormsg, "error: Failed to open file: '%s'", path);
	perror(errormsg);
	exit(FILE_OPEN_ERROR);
}

char *chkfgets(char *s, int n, FILE *iop, char *path)
{
	char *r = fgets(s, n, iop);
	if (!ferror(iop))
		return r;
	sprintf(errormsg, "error: Failed to read file: '%s'", path);
	perror(errormsg);
	fclose(iop);
	exit(FILE_READ_ERROR);
}
