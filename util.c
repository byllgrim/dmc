#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
die(char *msg)
{
	fprintf(stderr, msg);
	exit(EXIT_FAILURE);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	p = calloc(nmemb, size);
	if (!p)
		die("ecalloc: calloc failed");

	return p;
}
