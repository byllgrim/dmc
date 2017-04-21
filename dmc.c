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
	if (!(l->s)) /* TODO precedence */
		die("get_input: fgets failed\n");

	l->s[strlen(l->s) - 1] = '\0';
}

void
eot_token(struct token *t)
{
	t->type = EOT;
	t->text = (void *)0; /* TODO free text? set [0]=0 */
printf("eot_token\n");
}

void
number_token(struct token *t, struct line *l)
{
	(void)t; /* TODO */
	(void)l; /* TODO */

printf("number_token TODO\n"); /* TODO remove */
}

void
word_token(struct token *t, struct line *l)
{
	size_t i;
	/* TODO clear pointer for s */

	/* TODO | is allowed in word? */
	for (i = 0; l->s[l->pos + i] && !isspace(l->s[l->pos + i]); i++)
		; /* TODO make function? */

	memset(t->text, '\0', MAXWORDLEN); /* TODO explicit zero */
	strncpy(t->text, l->s + l->pos, i);
	t->type = WORD;
	l->pos += i;
printf("word_token '%s'\n", t->text); /* TODO remove */
}

void
char_token(struct token *t, struct line *l)
{
	t->type = l->s[l->pos];
	t->text[0] = l->s[l->pos];
	t->text[1] = '\0';
	l->pos++;
printf("char_token '%s'\n", t->text); /* TODO remove */
}

int
valid_char(char c)
{
	return c == '&' || c == '|' || c == '='
	       || c == '[' || c == ']';
}

void
get_token(struct token *t, struct line *l)
{
	char c;

	for (; isspace(l->s[l->pos]); l->pos++)
		; /* TODO skip_spaces() */

	c = l->s[l->pos];
	if (!c)
		eot_token(t);
	else if (isdigit(c))
		number_token(t, l);
	else if (valid_char(c))
		char_token(t, l); /* TODO explicit and first */
	else
		word_token(t, l);
}

struct tree_node *
new_node(void)
{
	struct tree_node *n;

	n = ecalloc(1, sizeof(*n));
	/* TODO does one alloc arrays in structs? */
	/* TODO unnecessary function */

	return n;
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
		die("match: unexpected token '%s'\n", t->text);

	/* TODO the advancement of get_token is a side-effect */
}

struct tree_node *
cmd_words(struct token *t, struct line *l)
{
	struct tree_node *n = new_node();

	n->t = copy_token(t);
	match(WORD, t, l);

	if (t->type == WORD)
		n->child[0] = cmd_words(t, l); /* TODO advance s */

	return n;
}

struct tree_node *
pipeto(struct token *t, struct line *l) /* pipe() clashes with unistd */
{
	struct tree_node *n = new_node();

	n->t = copy_token(t);
	match('|', t, l);

	/* if (t->type == '[') */

	return n;
}

struct tree_node *
sequence(struct token *t, struct line *l)
{
	struct tree_node *n = new_node();

	n->child[0] = cmd_words(t, l);

	if (t->type == '&') {
		n->t = copy_token(t); /* TODO asynchronous bg job */
		match('&', t, l);
	} else if (t->type == '|') {
		n->child[1] = pipeto(t, l); /* TODO advance s */
		n->child[2] = sequence(t, l);
	}

	/* TODO check if s is exhausted */
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
		wait((void*)0);
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
	l->s = ecalloc(BUFSIZ + 1, sizeof(*(l->s))); /* TODO pre */

	for (;;) {
		fputs(prompt, stdout);
		get_input(l, BUFSIZ);
		execute(l);
	}

	free(l->s);
	free(l);
	return 0;
}
