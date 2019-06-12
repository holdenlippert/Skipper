%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void
yyerror(const char *str)
{
	fprintf(stderr, "parser error: %s\n", str);
}

int
yywrap()
{
	return (1);
}

%}

%token ID LP RP SC

%union { char *name; void *node; }

%type<name> ID
%type<node> expr

%%

lines: /*empty*/
	| lines line SC
	;

line:
	ID expr { addinstr($1, $2); }
	|
	expr { addinstr(NULL, $1); }
	;

expr:
	ID { $$ = leaf($1); }
	|
	expr LP expr RP { $$ = node($1, $3);}
	;
