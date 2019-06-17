OBJECTS = ast.o treestack.o check.o dictionary.o interp.o y.tab.o lex.yy.o main.o 
CFLAGS = -Werror -g

mine: ${OBJECTS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJECTS} ${LIBS}

clean:
	-rm -f mine *.o core.* lex.yy.c y.tab.c y.tab.h

.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<

lex.yy.c: tokens.l
	lex tokens.l

y.tab.c: grammar.y
	yacc -d grammar.y
