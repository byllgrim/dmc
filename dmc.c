#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *
get_input(void)
{
	char *s;

	s = calloc(BUFSIZ + 1, sizeof(*s));
	fgets(s, BUFSIZ, stdin); /* TODO check return */
	s[strlen(s) - 1] = '\0';

	return s;
}

void
execute(char *s) /* TODO tokenize s (before?) */
{
	/* TODO fork */
	execlp(s, s, (void *)0);
}

int
main(void)
{
	char *s;

	for (;;) {
		s = get_input();
		execute(s);
		free(s);
	}

	return 0;
}
