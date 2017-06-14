CFLAGS=-lfl -lm

calc: lex.yy.o y.tab.o
	gcc -o calc lex.yy.o y.tab.o $(CFLAGS)

%.o: %.c
	gcc -c -o $@ $^ $(CFLAGS)

lex.yy.c: calc.l y.tab.c
	flex calc.l

y.tab.c: calc.y
	bison -vdty calc.y

