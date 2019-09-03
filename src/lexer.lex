%option nounput
%option noinput
%option noyywrap

%x code

%{
#include <iostream>
#include "src/variables.hpp"
#include "src/statement.hpp"

#include "parser.tab.hpp"

int line_counter = 0;
%}

NUM [0-9]
%%

"{%" {
    BEGIN(code);
    return begin_token;
}

<code>"%}" {
    BEGIN(INITIAL);
    return end_token;
}

<code>"true"|"True" return true_token;
<code>"false"|"False" return false_token;
<code>#[^\n]* {}

<code>[1-9]{NUM}*|{NUM}+(\.{NUM}+)? {
    yylval.num_type = atof(yytext);
    return num_token;
}

<code>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}

<code>\"[^\"]*\" {
    std::string s = std::string(yytext + 1);
    s = s.substr(0, s.size() - 1);
    yylval.str_type = new std::string(s);
    return string_token;
}

<code>[;\[\]=:{},#] return yytext[0];

<code>\n {
    line_counter++;
}

<code>[\t ] { }

<code>. {
    std::cout << "Error - Unknown character: " << *yytext << std::endl;
    exit(EXIT_FAILURE);
}

\n {
    line_counter++;
    ECHO;
}

. {
    ECHO;
}

%%
