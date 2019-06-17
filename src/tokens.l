%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
[a-zA-Z][a-zA-Z0-9]*	{ yylval.name = strdup(yytext); return ID; }
\(			return LP;
\)			return RP;
\;			return SC;
[ \t\n]			/* ignore whitespace */
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]       { /* DO NOTHING */ }
[/][*]                                    { printf("Unterminated comment"); exit(1);}
%%
