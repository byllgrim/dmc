#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(void)
{
	char *s;

	s = calloc(BUFSIZ + 1, sizeof(*s));
	for (;;) {
		fgets(s, BUFSIZ, stdin); /* TODO check return? */
		/* TODO getline(s, BUFSIZ) which does checking */
		s[strlen(s) - 1] = '\0';
		execlp(s, s, (void *)0);
	}

	return 0;
}
