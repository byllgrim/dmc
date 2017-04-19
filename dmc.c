#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

char *
get_input(void)
{
	char *s;

	s = ecalloc(BUFSIZ + 1, sizeof(*s));
	fgets(s, BUFSIZ, stdin); /* TODO check return */
	s[strlen(s) - 1] = '\0';

	return s;
}

void
execute(char *s) /* TODO tokenize s (before?) */
{
	switch (fork()) {
	case -1:
		die("execute: fork failed");
	case 0:
		execlp(s, s, (void *)0);
		/* TODO not found etc */
		break;
	default:
		wait((void*)0);
		/* TODO what about bg? */
	}
}

int
main(void)
{
	char *s;

	for (;;) {
		printf("dmc> "); /* TODO su prompt */
		s = get_input();
		execute(s);
		free(s);
	}

	return 0;
}
