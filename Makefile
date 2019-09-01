PROGRAM = markdown
CXX = g++
CFLAGS = -Wall -Wextra -std=c++11
PARSER = src/parser.ypp
LEXER = src/lexer.lex

$(PROGRAM): lex.yy.o parser.o
	$(CXX) $(CFLAGS) -o $@ $^

lex.yy.o: lex.yy.c parser.tab.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

lex.yy.c: $(LEXER)
	flex $<

parser.o: parser.tab.cpp parser.tab.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

parser.tab.hpp parser.tab.cpp: $(PARSER)
	bison -v -d $<

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM) lex.yy.c parser.tab.* parser.output
