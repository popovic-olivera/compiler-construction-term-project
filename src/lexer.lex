%option nounput
%option noinput
%option noyywrap

%x code
%x code_text

%{
#include <iostream>
#include <map>
#include <vector>
#include "src/variables.hpp"
#include "src/statement.hpp"
#include "src/colors.hpp"

#include "parser.tab.hpp"

int line_counter = 1; 
int text_counter = 0;
int line_characters_counter = 0;
std::vector<std::string> file_text{""};
std::map<int, int> added;
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
    std::string s = "Line " + std::to_string(line_counter) + ":Error - Unknown character: " + *yytext;
    std::cout << Color::set_green(s) << std::endl;
    exit(EXIT_FAILURE);
}

"{{" {
    BEGIN(code_text);
    return begin_text_token;
}

<code_text>"}}" {
    BEGIN(INITIAL);
    return end_text_token;
}

<code_text>"for" return for_token;
<code_text>"in" return in_token;
<code_text>"if" return if_token;

<code_text>[0-9]+ {
    yylval.int_type = atoi(yytext);
    return int_num_token;
}

<code_text>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}

<code_text>[\[\].] return yytext[0];

<code_text>\n {
    line_counter++;
}

\n {
    line_counter++;
    
    if(text_counter == 0)
        added[0] = 0;
    
    file_text[text_counter].append(yytext);
    text_counter++;
    line_characters_counter = 0;
    
    file_text.push_back("");
    added[text_counter] = 0;
}

. {
    file_text[text_counter].append(yytext);
    line_characters_counter++;
}

%%
