PROGRAM = markdown
CXX = g++
CFLAGS = -Wall -Wextra -std=c++11
PARSER = src/parser.ypp
LEXER = src/lexer.lex
VARH = src/variables.hpp
VAR = src/variables.cpp
STATH = src/statement.hpp
STAT = src/statement.cpp
COL = src/colors.hpp
COLH = src/colors.cpp

$(PROGRAM): lex.yy.o parser.o variables.o statement.o colors.o
	$(CXX) $(CFLAGS) -o $@ $^

variables.o: $(VAR) $(VARH) $(COL)
	$(CXX) $(CFLAGS) -c -o $@ $<

statement.o: $(STAT) $(STATH) $(COL)
	$(CXX) $(CFLAGS) -c -o $@ $<

colors.o: $(COLH) $(COL)
	$(CXX) $(CFLAGS) -c -o $@ $<
	
lex.yy.o: lex.yy.c parser.tab.hpp $(COL)
	$(CXX) $(CFLAGS) -c -o $@ $<

lex.yy.c: $(LEXER)
	flex $<

parser.o: parser.tab.cpp parser.tab.hpp $(COL)
	$(CXX) $(CFLAGS) -c -o $@ $<

parser.tab.hpp parser.tab.cpp: $(PARSER)
	bison -v -d $<

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM) lex.yy.c parser.tab.* parser.output
