#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "check.h"
#include "dictionary.h"
#include "main.h"
#include "treestack.h"

#define ISSYMBOL(tree) (tree->name != NULL)

/* Private global variable to track bindings: */
static Dict bindings;

void
initinterp()
{
	bindings = emptydict();
}

void
simplify(struct ast *expr)
{
	if (expr->name != NULL) {
		// We have a symbol.
		struct ast *value = get(bindings, expr->name);
		if (value == NULL)
			// The symbol is a built-in.
			return;
		if (value->name != NULL) {
			expr->name = value->name;
			return;
		}
		expr->function = deepcopy(value->function);
		expr->argument = deepcopy(value->argument);
		expr->name = NULL;
	}
	simplify(expr->function);
	simplify(expr->argument);
}

void
evaluate(struct ast *expr, bool debug_flag)
{
	// This function assumes that all macros in expr have already been
	// expanded.
	checktree(expr, false);
	if (debug_flag) {
		printf("\nevaluating: ");
		printtree(expr);
		printf("\n");
	}

	struct treestack *stack = newstack();
	pushtree(stack, expr);
	struct ast *current, *x, *y, *z;
	while (!isempty(stack)) {
		current = poptree(stack);
		if (debug_flag) {
			printf("\nPopped %p: ", current);
			checktree(current, false);
			printtree(current);
			printf("\n");
		}
		checktree(current, false);
		
		if (ISSYMBOL(current)) {
			if (debug_flag)
				printf("hit a symbol\n");
			// Just a symbol, nothing to do.
			continue;
		}
		if (!ISSYMBOL(current->function)) {
			// Function still needs to be simplified. Do that first
			// and then try again.
			if (debug_flag) {
				printf("Hit an application of an application, pushing: %p_", current);
				checktree(current, false);
				printtree(current);
				printf(" and %p_", current->function);
				checktree(current->function, false);
				printtree(current->function);
				printf(".\n");
			}
			pushtree(stack, current);
			pushtree(stack, current->function);
			continue;
		}
		if (strcmp(current->function->name, "p") == 0) {
			if (debug_flag) {
				printf("Hit an application of p, moving %p to %p\n", current->argument, current);
			}
			printf(".");
			memcpy(current, current->argument, sizeof(struct ast));
		} else if (strcmp(current->function->name, "k") == 0) {
			if (debug_flag) {
				printf("hit an application of k");
			}
			if (current->function->param1 != NULL) {
				if (debug_flag) {
					printf("with one param, moving %p to %p\n", current->function->param1, current);
				}
				memcpy(current, current->function->param1,
				    sizeof(struct ast));
			}
			else {
				if (debug_flag) {
					printf("with no param, moving %p to %p\n", current->argument, current->function->param1);
				}
				current->function->param1 = malloc(sizeof(struct ast));
				memcpy(current->function->param1,
				    current->argument, sizeof(struct ast));
				memcpy(current, current->function, sizeof(struct ast));
			}
		} else if (strcmp(current->function->name, "s") == 0) {
			if (debug_flag) {
				printf("hit an application of s");
			}
			if (current->function->param2 != NULL) {
				if (debug_flag) {
					printf("with two params\n");
				}
				x = current->function->param1;
				y = current->function->param2;
				z = current->argument;

				current->name = NULL;
				current->function->name = NULL;
				current->function->function = x;
				current->function->argument = z;

				current->argument = malloc(sizeof(struct ast));
				current->argument->name = NULL;
				current->argument->argument = z;
				current->argument->function = y;
			} else if (current->function->param1 != NULL) {
				if (debug_flag) {
					printf("with one param\n");
				}
				current->function->param2 = malloc(sizeof(struct ast));
				memcpy(current->function->param2,
				    current->argument, sizeof(struct ast));
				memcpy(current, current->function, sizeof(struct ast));
			} else {
				if (debug_flag) {
					printf("with no params\n");
				}
				current->function->param1 = malloc(sizeof(struct ast));
				memcpy(current->function->param1,
				    current->argument, sizeof(struct ast));
				memcpy(current, current->function, sizeof(struct ast));
			}
		} else {
			printf("unrecognized symbol: %s\n", current->function->name);
			exit(1);
		}
		if (debug_flag) {
			checktree(current, false);
			printf("pushing %p onto the stack\n", current);
		}
		pushtree(stack, current);
	}

}

void
execute(struct instruction *instr, bool simplify_flag, bool debug_flag)
{
	if (instr->name) {
		insert(bindings, instr->name, instr->expr);
	}
	else {
		simplify(instr->expr);
		if (simplify_flag) {
			printtree(instr->expr);
		}
		else
			evaluate(instr->expr, debug_flag);
		printf("\n");
	}
}

