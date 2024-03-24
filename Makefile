ifeq ($(DEBUG), 2)
	CFLAGS = -DDEBUG -g -Wall
	FXFLAGS = -d
	BSFLAGS = -t
else ifeq ($(DEBUG), 1)
	CFLAGS = -DDEBUG -g -Wall
	FXFLAGS =
	BSFLAGS =
else
	CFLAGS = -O2 -Wall
	FXFLAGS =
	BSFLAGS =
endif

SOURCES = \
	lexer.yy.cpp \
	ast.cpp \
	type.cpp \
	ir.cpp \
	c2.cpp

.PHONY: all
all: scanner parser translator

.PHONY: scanner
scanner: $(SOURCES) lexer.yy.cpp scanner.cpp | parser.tab.cpp
	g++ -o $@ $^ -lfl $(CFLAGS)

.PHONY: parser
parser: $(SOURCES) lexer.yy.cpp parser.tab.cpp parser.cpp
	g++ -o $@ $^ -lfl -ly -lm $(CFLAGS)

.PHONY: translator
translator: $(SOURCES) lexer.yy.cpp parser.tab.cpp translator.cpp
	g++ -o $@ $^ -lfl -ly -lm $(CFLAGS)

.PHONY: graph
graph: parser.ypp
	bison -v $(BSFLAGS) --graph $^
	dot -Tpng parser.dot -o parser.png

parser.tab.cpp: parser.ypp
	bison -v $(BSFLAGS) $^

lexer.yy.cpp: lexer.l
	flex -C -o $@ $(FXFLAGS) $^

.PHONY: clean
clean:
	rm -f *.o *.yy.c *.yy.cpp *.tab.c *.tab.h *.tab.cpp *.tab.hpp scanner parser translator parser.output parser.dot parser.png
