mine: tokens.l grammar.y
	lex tokens.l; yacc -d grammar.y; gcc lex.yy.c y.tab.c -o mine
