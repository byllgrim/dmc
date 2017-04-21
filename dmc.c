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
/* TODO killyourself. Your data structures and architecture suck */

struct line { /* TODO input_line, sequence, statement? */
	size_t pos;
	char *s;
};

void
get_input(struct line *l, size_t len)
{
	l->s = fgets(l->s, len, stdin);
	if (!l->s)
		die("get_input: fgets failed\n");

	l->s[strlen(l->s) - 1] = '\0';
}

void
number_token(struct token *t, struct line *l)
{
	size_t i;
	char *s;

	s = l->s + l->pos;
	for (i = 0; s[i] && isdigit(s[i]); i++)
		;

	strncpy(t->text, s, i);
	t->text[i] = '\0';
	t->type = NUMBER;
	l->pos += i;

	/* TODO function pointer and combine word_token? */
}

void
word_token(struct token *t, struct line *l)
{
	size_t i;
	char *s;

	/* TODO | is allowed in word? */
	s = l->s + l->pos;
	for (i = 0; s[i] && !isspace(s[i]); i++)
		; /* TODO make function? */

	strncpy(t->text, s, i);
	t->text[i] = '\0'; /* memset(t->text, '\0', MAXWORDLEN); */
	t->type = WORD;
	l->pos += i;
	/* TODO what about "words in quotes"? */
}

void
char_token(struct token *t, struct line *l)
{
	char *s = l->s + l->pos;
	t->type = s[0];
	t->text[0] = s[0];
	t->text[1] = '\0';
	l->pos++;
}

int
valid_char(char c) /* TODO valid_single()? */
{
	return c == '&' || c == '|' || c == '='
	       || c == '[' || c == ']';
}

void
get_token(struct token *t, struct line *l)
{
	char c;

	for (; isspace(l->s[l->pos]); l->pos++)
		;

	c = l->s[l->pos];
	if (!c)
		t->type = EOT;
	else if (isdigit(c))
		number_token(t, l);
	else if (valid_char(c))
		char_token(t, l); /* TODO explicit and first */
	else
		word_token(t, l);
}

struct token *
copy_token(struct token *t)
{
	struct token *new;

	new = ecalloc(1, sizeof(*new));
	new->text = ecalloc(MAXWORDLEN + 1, sizeof(char));

	new->type = t->type;
	strncpy(new->text, t->text, MAXWORDLEN);

	return new;
}

void
match(int expected, struct token *t, struct line *l)
{
	if (t->type == expected)
		get_token(t, l);
	else
		die("match: expected %d got %d '%s'\n",
		    expected, t->type, t->text);

	/* TODO the advancement of get_token is a side-effect */
	/* TODO allow specifying error message? */
}

struct tree_node *
cmd_words(struct token *t, struct line *l)
{
	struct tree_node *n = ecalloc(1, sizeof(*n));

	n->t = copy_token(t);
	match(WORD, t, l);

	if (t->type == WORD)
		n->child[0] = cmd_words(t, l); /* TODO advance s */

	return n;
}

struct tree_node *
redirection(struct token *t, struct line *l)
{
	struct tree_node *n = ecalloc(1, sizeof(*n));

	match('[', t, l);

	n->t = copy_token(t);
	match(NUMBER, t, l);

	if (t->type == '=') {
		match('=', t, l);

		n->t = copy_token(t); /* TODO copy to new t */
		match(NUMBER, t, l);
	}

	match(']', t, l);

	return n;
}

struct tree_node *
pipeto(struct token *t, struct line *l) /* pipe() clashes with unistd */
{
	struct tree_node *n = ecalloc(1, sizeof(*n));

	n->t = copy_token(t);
	match('|', t, l);

	if (t->type == '[')
		n->child[0] = redirection(t, l);

	return n;
}

struct tree_node *
sequence(struct token *t, struct line *l)
{
	struct tree_node *n = ecalloc(1, sizeof(*n));

	n->child[0] = cmd_words(t, l);

	if (t->type == '&') {
		n->t = copy_token(t); /* TODO asynchronous bg job */
		match('&', t, l);
	} else if (t->type == '|') {
		n->child[1] = pipeto(t, l);
		n->child[2] = sequence(t, l);
	}

	/* TODO check if l is exhausted */
	return n;
}

struct tree_node *
parse(struct line *l)
{
	struct token *t;

	t = ecalloc(1, sizeof(*t));
	t->text = ecalloc(MAXWORDLEN + 1, sizeof(char));
	get_token(t, l);
	return sequence(t, l); /* TODO */
}

void
walk_tree(struct tree_node *ast)
{
	/* TODO parse entire tree */
	execlp(ast->child[0]->t->text,
	       ast->child[0]->t->text,
	       (void *)0);
}

void
execute(struct line *l)
{
	struct tree_node *ast; /* abstract syntax tree */

	switch (fork()) {
	case -1:
		die("execute: fork failed\n");
	case 0:
		ast = parse(l);
		walk_tree(ast);
		break;
	default:
		wait((void*)0); /* TODO specify child PID? */
		/* TODO what about bg? */
	}
}

int
main(void)
{
	struct line *l;
	char prompt[] = "dmc> "; /* TODO proper prompt */

	/* TODO ignore interrupt signal etc */
	l = ecalloc(BUFSIZ + 1, sizeof(*l));
	l->s = ecalloc(BUFSIZ + 1, sizeof(*l->s));

	for (;;) {
		fputs(prompt, stdout);
		get_input(l, BUFSIZ);
		execute(l);
	}

	free(l->s);
	free(l);
	return 0;
}
