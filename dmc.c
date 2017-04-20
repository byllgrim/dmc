#include <sys/wait.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

enum {MAXCHILDREN = 3 /* TODO */, MAXWORDLEN = 64};

enum token_type {
	/* ascii printables e.g. '&' are valid types */
	WORD = 128, /* start after ascii */
	NUMBER,
	EOT /* end of transmission */
};

struct token {
	int type;
	char *text;
};

struct tree_node {
	struct tree_node *child[MAXCHILDREN];
	struct token *t;
};

char *
get_input(char *s, size_t len)
{
	s = fgets(s, len, stdin);
	if (!s)
		die("get_input: fgets failed\n");

	s[strlen(s) - 1] = '\0';

	return s;
}

size_t
eot_token(struct token *t)
{
	t->type = EOT;
	t->text = (void *)0;
	return 0;
}

size_t
number_token(struct token *t, char *s)
{
	(void)t; /* TODO */
	(void)s; /* TODO */

printf("number_token TODO\n"); /* TODO remove */
	return 0; /* TODO */
}

size_t
word_token(struct token *t, char *s)
{
	int i;

	for (i = 0; s[i] && !isspace(s[i]); i++)
		; /* TODO make function? */

	strncpy(t->text, s, i);
	t->type = WORD;
printf("word_token '%s'\n", t->text); /* TODO remove */
	return i;
}

size_t
char_token(struct token *t, char *s)
{
	t->type = s[0];
	t->text[0] = s[0];
	t->text[1] = '\0';
printf("char_token '%c'\n", s[0]); /* TODO remove */
	return 1;
}

size_t
get_token(struct token *t, char *s)
{
	for (; isspace(s[0]); s++)
		;

	if (!s[0])
		return eot_token(t);
	else if (isdigit(s[0]))
		return number_token(t, s);
	else if (isalpha(s[0]))
		return word_token(t, s);
	else
		return char_token(t, s); /* TODO explicit and first */
}

struct tree_node *
new_node(void)
{
	struct tree_node *n;

	n = ecalloc(1, sizeof(*n));
	/* TODO does one alloc arrays in structs? */

	return n;
}

struct token *
copy_token(struct token *t)
{
	struct token *new;

	new = ecalloc(1, sizeof(*new));
	memcpy(new, t, sizeof(*new));

	return new;
}

void
match(int expected, struct token *t, char *s)
{
	if (t->type == expected)
		s += get_token(t, s);
	else
		die("match: unexpected token '%s'\n", t->text);

	/* TODO the advancement of get_token is a side-effect */
}

struct tree_node *
cmd_words(struct token *t, char *s)
{
	struct tree_node *n = new_node();

	n->t = copy_token(t);
	match(WORD, t, s);

	if (t->type == WORD) {
		n->child[0] = cmd_words(t, s); /* TODO advance s */
	}

	return n;
}

struct tree_node *
sequence(struct token *t, char *s)
{
	struct tree_node *n = new_node();

	n->child[0] = cmd_words(t, s);

	if (t->type == '&') {
		n->t = copy_token(t); /* TODO asynchronous bg job */
		match('&', t, s);
	} else if (t->type == '|') {
		/*
		TODO
		n->child[1] = pipe();
		n->child[2] = sequence();
		*/
	}

	/* TODO check if s is exhausted */
	return n;
}

struct tree_node *
parse(char *s)
{
	struct token *t;

	t = ecalloc(1, sizeof(*t));
	t->text = ecalloc(MAXWORDLEN + 1, sizeof(char));
	s += get_token(t, s);
	return sequence(t, s); /* TODO */
}

void
execute(char *s)
{
	struct tree_node *ast; /* abstract syntax tree */

	switch (fork()) {
	case -1:
		die("execute: fork failed\n");
	case 0:
		ast = parse(s);
		execlp(s, s, (void *)0); /* TODO walk_tree(ast) */
		(void)ast; /* TODO */
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
	char prompt[] = "dmc> "; /* TODO proper prompt */

	/* TODO ignore interrupt signal etc */
	s = ecalloc(BUFSIZ + 1, sizeof(*s));

	for (;;) {
		fputs(prompt, stdout);
		get_input(s, BUFSIZ);
		execute(s);
	}

	free(s);
	return 0;
}
