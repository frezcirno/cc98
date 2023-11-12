ifeq ($(DEBUG), 1)
	CFLAGS = -DDEBUG -g -Wall
	FXFLAGS = -d
	BSFLAGS = -t
else
	CFLAGS = -O2 -Wall
	FXFLAGS =
	BSFLAGS =
endif

default: parser scanner

parser: flex bison
	gcc -o parser parser.c parser.tab.c lexer.yy.c ast.c -lfl -ly -lm $(CFLAGS)

scanner: flex bison
	gcc -o scanner scanner.c lexer.yy.c ast.c -lfl $(CFLAGS)

graph:
	bison -v parser.y $(BSFLAGS) --graph
	dot -Tpng parser.dot -o parser.png

bison: parser.y
	bison -v parser.y $(BSFLAGS)

flex: lexer.l
	flex -o lexer.yy.c $(FXFLAGS) lexer.l

clean:
	rm -f *.o *.yy.c *.tab.c *.tab.h scanner parser parser.output parser.dot parser.png
