#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "check.h"
#include "main.h"

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
	ast->param1 = NULL;
	ast->param2 = NULL;
	ast->color = 0;
	return ast;
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
	return ast;
}

/*
 * Recursively copy 'tree' and all of its children. Note: the string name is
 * not copied.
 */
struct ast *
deepcopy(struct ast *tree)
{
	struct ast *new;
	if (tree == NULL)
		return NULL;
	new = malloc(sizeof(struct ast));
	new->name = tree->name;
	new->function = deepcopy(tree->function);
	new->argument = deepcopy(tree->argument);
	return new;
}

/*
 * Print out the AST 'tree' in human readable form.
 */
void
printtree(struct ast *tree)
{
	checktree(tree, false);
	if (tree->name != NULL) {
		printf("%s", tree->name);
		if (tree->param1 != NULL) {
			printf("[");
			printtree(tree->param1);
			if (tree->param2 != NULL) {
				printf(", ");
				printtree(tree->param2);
			}
			printf("]");
		}
	}
	else {
		printtree(tree->function);
		printf("(");
		printtree(tree->argument);
		printf(")");
	}
}
