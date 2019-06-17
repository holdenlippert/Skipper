#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "main.h"
#include "ast.h"
#include "treestack.h"

static void checksymbolorapp(struct ast *tree);

void
whiteout(struct ast *tree)
{
	if (tree == NULL)
		return;
	tree->color = 'w';
	whiteout(tree->function);
	whiteout(tree->argument);
}

void
checktree(struct ast *tree, bool verbose_flag)
{
	struct treestack *stack;
	struct ast *current;

	// Initialize the stack:
	stack = newstack();
	pushtree(stack, tree);


	// Run DFS on the graph and check for correctness:
	if (verbose_flag)
		printf("\n\nAbout to run check...\n");
	while (!isempty(stack)) {
		// Perform check on head:
		current = poptree(stack);
		if (current->name != NULL) {
			if (verbose_flag)
				printf("Checking %p(%s)\n", current,
				    current->name);
		} else {
			if (verbose_flag)
				printf("Checking %p(%p, %p)\n", current,
				    current->function, current->argument);
		}

		// Assure symbols and applications are mutually exclusive:
		if (verbose_flag)
			printf("checking for mututal exclusion\n");
		checksymbolorapp(current);
		if (verbose_flag)
			printf("mututal exclusion good\n");

		// We should only find each tree once (no loops!):

		// Push children:
		if (current->name == NULL) {
			pushtree(stack, current->argument);
			pushtree(stack, current->function);
		}
	}
	whiteout(tree);
	if (verbose_flag)
		printf("Finished checking\n");

}

static void
checksymbolorapp(struct ast *tree)
{
	if (tree->name == NULL) {
		assert(tree->function != NULL);
		assert(tree->argument != NULL);
	} else {
		assert(tree->function == NULL);
		assert(tree->argument == NULL);
	}
	if (tree->function == NULL) {
		assert(tree->name != NULL);
		assert(tree->argument == NULL);
	} else {
		assert(tree->name == NULL);
		assert(tree->argument != NULL);
	}
	if (tree->argument == NULL) {
		assert(tree->name != NULL);
		assert(tree->function == NULL);
	} else {
		assert(tree->name == NULL);
		assert(tree->function != NULL);
	}
}
