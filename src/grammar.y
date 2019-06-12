%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char *

void yyerror(const char *str)
{
	fprintf(stderr, "parser error: %s\n", str);
}

int yywrap() { return (1); }

int main() { yyparse(); }
%}

%token ID LP RP SC

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
	ID expr { printf("\tGot bind: %s\n", $1); }
	;

expr:
	ID
	|
	expr LP expr RP { printf("\tGot expr!\n"); }
	;
