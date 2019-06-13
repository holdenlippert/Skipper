mine: tokens.l grammar.y main.h main.c dictionary.h dictionary.c
	lex tokens.l; yacc -d grammar.y; gcc lex.yy.c y.tab.c main.c dictionary.c -o mine
