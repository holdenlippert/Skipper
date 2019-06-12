%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
[a-zA-Z][a-zA-Z0-9]*	{ yylval=strdup(yytext); return ID; }
\(			return LP;
\)			return RP;
\;			return SC;
%%
