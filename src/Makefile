mine: tokens.l grammar.y main.h main.c
	lex tokens.l; yacc -d grammar.y; gcc lex.yy.c y.tab.c main.c -o mine
