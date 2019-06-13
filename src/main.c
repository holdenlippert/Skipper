#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "y.tab.h"
#include "main.h"
#include "dictionary.h"

/* Struct definitions: */

/**
 * Defines a linked list of bind/evaluate instructions. If the 'name' field is
 * NULL, the instruction is to evaluate 'expr'; otherwise the instruction is to
 * bind 'expr' to 'name'.
 */
struct instruction {
	char *name;
	struct ast *expr;
	struct instruction *next;
};

/**
 * Defines the AST structure of the program. Every node is either an
 * application or a symbol. If 'name' is null, then it represents an
 * application of 'function' onto 'argument'. If 'name' is not null, then it
 * represents a symbol with name 'name'.
 */
struct ast {
	char *name;
	struct ast *function;
	struct ast *argument;
};

/* Private function prototypes: */
static void			printtree(struct ast *);
static struct instruction *	getinstr();
static void			printinstr(struct instruction *instr);

static void printsymbol(struct ast *);
static void simplify(struct ast *);

/* Private global variables to keep track of the instruction queue: */
static struct instruction *first = NULL;
static struct instruction *last = NULL;

/* Private global variable to track bindings: */
static Dict bindings;

/*
 * Create a new AST node that represents the application of 'function' onto
 * 'argument'.
 */
struct ast *
node(struct ast *function, struct ast *argument)
{
	struct ast *ast = malloc(sizeof(struct ast));
	ast->name = NULL;
	ast->function = function;
	ast->argument = argument;
}

/*
 * Create a new AST node that represents the symbol with name 'name'.
 */
struct ast *
leaf(char *name)
{
	struct ast *ast = malloc(sizeof(struct ast));
	ast->name = strdup(name);
	ast->function = NULL;
	ast->argument = NULL;
}

/*
 * Print out the AST 'tree' in human readable form.
 */
static void
printtree(struct ast *tree)
{
	if (tree->name != NULL)
		printf("%s", tree->name);
	else {
		printtree(tree->function);
		printf("(");
		printtree(tree->argument);
		printf(")");
	}
}

/*
 * Add an instruction to the instruction queue. If 'name' is null, it is an
 * evaluate instruction; otherwise it is a bind instruction.
 */
void
addinstr(char *name, struct ast *expr)
{
	struct instruction *new = malloc(sizeof(struct instruction));
	if (name != NULL)
		new->name = strdup(name);
	new->expr = expr;
	new->next = NULL;

	if (first == NULL)
		first = new;

	if (last == NULL)
		last = new;
	else {
		last->next = new;
		last = new;
	}
}

/*
 * Remove and return an instruction from the instruction queue.
 */
static struct instruction *
getinstr()
{
	struct instruction *ret = first;
	first = first->next;
	return (ret);
}

/*
 * Print out a human readable interpretation of an instruction.
 */
void
printinstr(struct instruction *instr)
{
	if (instr->name != NULL) {
		printf("Bind %s to ", instr->name);
	}
	else {
		printf("Evaluate ");
	}
	printtree(instr->expr);
}

void
evaluate(struct ast *expr)
{

}

void
execute(struct instruction *instr, bool simplify_flag)
{
	if (instr->name)
		insert(bindings, instr->name, instr->expr);
	else {
		if (simplify_flag) {
			simplify(instr->expr);
			printf("\n");
		}
		else
			evaluate(instr->expr);
	}
}

void
printsymbol(struct ast *symbol)
{
	struct ast * value = get(bindings, symbol->name);
	if (value == NULL)
		printf("%s", symbol->name);
	else
		simplify(value);
}

void
simplify(struct ast *expr)
{
	if (expr->name != NULL)
		printsymbol(expr);
	else {
		simplify(expr->function);
		printf("(");
		simplify(expr->argument);
		printf(")");
	}
}

void
usage(char *program)
{
	printf("usage: %s [-s] [filename]\n", program);
	exit(1);
}

int
main(int argc, char **argv) {
	int c, fd;
	extern int optind;
	bool simplify_flag = false;

	/* Process command line arguments: */
	while ((c = getopt(argc, argv, "s")) != -1) {
		switch (c) {
		case 's':
			simplify_flag = true;
			break;
		default:
			usage(argv[0]);
		}
	}
	
	/* Open the file if provided (use stdin if not: */
	if (optind == argc) // No extra args, use stdin.
		fd = 0;
	else if (optind == argc - 1) // One extra arg, use as input.
		fd = open(argv[optind], O_RDONLY);
	else
		usage(argv[0]); // Wrong number of arguments.
	if (fd < 0)
		usage(argv[0]); // Invalid filename.
	dup2(fd, 0); // Redirect input from file to stdin.

	bindings = emptydict();

	// Parse the program using code generated by yacc:
	yyparse();
	printf("finished parsing\n");

	// Print out the parsed code:
	while (first != NULL) {
		execute(getinstr(), simplify_flag);
	}

	printdict(bindings);

	return (0);
}