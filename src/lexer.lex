%option nounput
%option noinput
%option noyywrap

%{
#include <iostream>

%}

%%

[\n\t ] { }

. {
    std::cout << "Error - Unknown character: " << *yytext << std::endl;
    exit(EXIT_FAILURE);
}

%%
