ifeq ($(DEBUG), 1)
	CFLAGS = -DDEBUG -g -Wall -I ast
	FXFLAGS = -d
	BSFLAGS = -t
else
	CFLAGS = -O2 -Wall -I ast
	FXFLAGS =
	BSFLAGS =
endif

SRC = lexer.yy.cpp ast.cpp type.cpp ir.cpp

.PHONY: default
default: scanner parser translator

scanner: flex bison
	g++ scanner.cpp $(SRC) -lfl $(CFLAGS) -o scanner

parser: flex bison
	g++ parser.cpp parser.tab.cpp $(SRC) -lfl -ly -lm $(CFLAGS) -o parser

translator: flex bison
	g++ translator.cpp parser.tab.cpp $(SRC) -lfl -ly -lm $(CFLAGS) -o translator

graph:
	bison -v parser.ypp $(BSFLAGS) --graph
	dot -Tpng parser.dot -o parser.png

bison: parser.ypp
	bison -v parser.ypp $(BSFLAGS)

flex: lexer.l
	flex -C -o lexer.yy.cpp $(FXFLAGS) lexer.l

clean:
	rm -f *.o *.yy.c *.yy.cpp *.tab.c *.tab.h *.tab.cpp *.tab.hpp scanner parser parser.output parser.dot parser.png
