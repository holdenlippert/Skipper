%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ast {
	char *name;
	struct ast *function;
	struct ast *argument;
};

struct ast *root = NULL;

struct ast *
node(struct ast *function, struct ast *argument)
{
	//printf("calling node\n");
	struct ast *ast = malloc(sizeof(struct ast));
	ast->name = NULL;
	ast->function = function;
	ast->argument = argument;
	root = ast;
}

struct ast *
leaf(char *name)
{
	struct ast *ast = malloc(sizeof(struct ast));
	ast->name = strdup(name);
	ast->function = NULL;
	ast->argument = NULL;
}

void
printtree(struct ast *tree)
{
	//printf("calling printtree on %p\n", tree);
	//printf("calling printtree on (%p = %s, %p, %p)\n", tree->name, tree->name, tree->function, tree->argument);
	if (tree->name != NULL)
		printf("%s", tree->name);
	else {
		printtree(tree->function);
		printf("(");
		printtree(tree->argument);
		printf(")");
	}
}

void yyerror(const char *str)
{
	fprintf(stderr, "parser error: %s\n", str);
}

int yywrap() { return (1); }

int main() {
	yyparse();
	printf("finished parsing\n");
	printtree(root);
	printf("\n");
}
%}

%token ID LP RP SC

%union { char *name; struct ast *node; }

%type<name> ID
%type<node> expr

%%

lines: /*empty*/
	| lines line SC
	;

line:
	bind
	|
	expr { printtree(root); printf("\n"); }
	;

bind:
	ID expr { printf("got bind.\n");}
	;

expr:
	ID { $$ = leaf($1); }
	|
	expr LP expr RP { $$ = node($1, $3);}
	;
