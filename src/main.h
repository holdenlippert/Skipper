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

void addinstr(char *name, struct ast *expr);
