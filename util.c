#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
die(char *msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	p = calloc(nmemb, size);
	if (!p)
		die("ecalloc: calloc failed\n");

	return p;
}
