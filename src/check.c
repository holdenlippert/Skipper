#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "main.h"
#include "ast.h"
#include "treestack.h"

static void checksymbolorapp(struct ast *tree);
static void checkloop(struct ast *tree);

void
whiteout(struct ast *tree)
{
	if (tree == NULL)
		return;
	tree->color = 0;
	whiteout(tree->function);
	whiteout(tree->argument);
}

void
checktree(struct ast *tree, bool verbose_flag)
{
	checkloop(tree);
	whiteout(tree);
}

static void
checkloop(struct ast *tree)
{
	if (tree == NULL || tree->color == 2)
		return;
	checksymbolorapp(tree);
	assert(tree->color == 0);
	tree->color = 1;
	checkloop(tree->function);
	checkloop(tree->argument);
	checkloop(tree->param1);
	checkloop(tree->param2);
	tree->color = 2;
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
