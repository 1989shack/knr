/* Exercise 4-5
 *
 * Add access to library functins like sin, exp, and pow.  See <math.h> in 
 * Appendix B, Section 4.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#define MAXOP 100
#define NUMBER '0'
#define WORD '1'

#define MAXVAL 100

#define BUFSIZE 100

int apply(char[]);
int getop(char[]);
void push(double);
double pop(void);
void peek(void);
void duplicate(void);
void swap(void);
void clear(void);

int sp = 0;
double val[MAXVAL];

int getch(void);
void ungetch(int);

char buf[BUFSIZE];
int bufp = 0;

int main(void)
{
	int type;
	double op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case WORD:
			apply(s);
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			op2 = pop();
			if (op2 != 0.0)
				push((long)pop() % (long)op2);
			else
				printf("error: zero divisor (mod)\n");
			break;
		case '?':
			peek();
			break;
		case '"':
			duplicate();
			break;
		case '~':
			swap();
			break;
		case '`':
			clear();
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}

int apply(char word[])
{
	double op2;

	if (strcmp(word, "sin") == 0) {
		push(sin(pop()));
	} else if (strcmp(word, "cos") == 0) {
		push(cos(pop()));
	} else if (strcmp(word, "tan") == 0) {
		push(tan(pop()));
	} else if (strcmp(word, "asin") == 0) {
		push(asin(pop()));
	} else if (strcmp(word, "acos") == 0) {
		push(acos(pop()));
	} else if (strcmp(word, "atan") == 0) {
		push(atan(pop()));
	} else if (strcmp(word, "atan2") == 0) {
		op2 = pop();
		push(atan2(pop(), op2));
	} else if (strcmp(word, "exp") == 0) {
		push(exp(pop()));
	} else if (strcmp(word, "log") == 0) {
		push(log(pop()));
	} else if (strcmp(word, "pow") == 0) {
		op2 = pop();
		push(pow(pop(), op2));
	} else if (strcmp(word, "sqrt") == 0) {
		push(sqrt(pop()));
	} else if (strcmp(word, "ceil") == 0) {
		push(ceil(pop()));
	} else if (strcmp(word, "floor") == 0) {
		push(floor(pop()));
	} else {
		return 0;
	}
	return 1;
}

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

void peek(void)
{
	if (sp > 0)
		printf("peek: %g\n", val[sp - 1]);
	else
		printf("error: stack empty\n");
}

void duplicate(void)
{
	if (sp <= 0)
		printf("error: stack empty\n");
	else if (sp >= MAXVAL)
		printf("error: stack full, can't duplicate\n");
	else
		push(val[sp - 1]);
}

void swap(void)
{
	int t;
	if (sp <= 1) {
		printf("error: stack has fewer than two items\n");
	} else {
		t = val[sp - 2];
		val[sp - 2] = val[sp - 1];
		val[sp - 1] = t;
	}
}

void clear(void)
{
	sp = 0;
}

int getop(char s[])
{
	int i, c, next;

	i = 0;
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	if (isalpha(c)) {
		while (isalnum(s[++i] = c = getch()))
			;
		s[i] = '\0';
		if (c != EOF)
			ungetch(c);
		return WORD;
	}
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;
	if (c == '-') {
		ungetch(next = getch());
		if (!isdigit(next) && next != '.')
			return c;
	}
	if (isdigit(c) || c == '-')
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

/* Sample

2 5 pow
    32
    
1 sin
    0.84147098

*/
