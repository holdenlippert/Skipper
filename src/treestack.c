#include <stdbool.h>
#include <stdlib.h>
#include "ast.h"

struct treelist {
	struct ast *tree;
	struct treelist *next;
};

struct treestack {
	struct treelist *head;
};

struct treestack *
newstack()
{
	struct treestack *new = malloc(sizeof(struct treestack));
	new->head = NULL;
	return new;
}

void
pushtree(struct treestack *stack, struct ast *tree)
{
	struct treelist *new = malloc(sizeof(struct treelist));
	new->tree = tree;
	new->next = stack->head;
	stack->head = new;
}

struct ast *
poptree(struct treestack *stack)
{
	if (stack->head == NULL)
		return NULL;
	struct ast *elem = stack->head->tree;
	stack->head = stack->head->next;
	return elem;
}

bool
isempty(struct treestack *stack)
{
	return (stack->head == NULL);
}
