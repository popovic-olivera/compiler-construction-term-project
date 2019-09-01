%option nounput
%option noinput
%option noyywrap

%{
#include <iostream>

#include "parser.tab.hpp"

bool code = false;
%}

NUM [0-9]
%%

"{%" {
    code = true;
    return begin_token;
}

"%}" {
    code = false;
    return end_token;
}

"true"|"True" return true_token;
"false"|"False" return false_token;

#[^\n]* return comment_token;

[1-9]{NUM}*|{NUM}+(\.{NUM}+)? {
    yylval.num_type = atof(yytext);
    return num_token;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}

\"[^\"]*\" {
    std::string s = std::string(yytext + 1);
    s = s.substr(0, s.size() - 1);
    yylval.str_type = new std::string(s);
    return string_token;
}

[\n\[\]=:{},#] return yytext[0];

[\t ] { }

. {
    std::cout << "Error - Unknown character: " << *yytext << std::endl;
    exit(EXIT_FAILURE);
}

%%
