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
	struct ast *param1;
	struct ast *param2;
	char color;
};

struct ast * node(struct ast *, struct ast *);
struct ast * leaf(char *name);
struct ast * deepcopy(struct ast *tree);
void printtree(struct ast *tree);
