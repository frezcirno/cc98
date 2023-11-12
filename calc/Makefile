ifeq ($(DEBUG), 1)
	BSFLAGS = -t
else
	BSFLAGS =
endif

all: lex bison
	gcc -o parser main.c syntax.tab.c -lfl -ly -lm

bison:
	bison -v -d syntax.y $(BSFLAGS)

lex: bison
	flex lex.l

clean:
	rm -f *.o lex.yy.c bison.tab.c bison.tab.h scanner parser
