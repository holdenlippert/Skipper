%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str)
{
	fprintf(stderr, "parser error: %s\n", str);
}

int yywrap() { return (1); }

int main() { yyparse(); }
%}

%token ID LP RP SC

%union { char *name; }

%%

lines: /*empty*/
	| lines line SC
	;

line:
	bind
	|
	expr
	;

bind:
	ID expr { printf("\tGot bind.\n"); }
	;

expr:
	ID
	|
	expr LP expr RP { printf("\tGot expr!\n"); }
	;
