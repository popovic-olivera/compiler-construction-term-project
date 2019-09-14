%option nounput
%option noinput
%option noyywrap

%x code
%x code_text
%x if_text
%x if_stat

%{
#include <iostream>
#include <map>
#include <vector>

#include "src/variables.hpp"
#include "src/statement.hpp"
#include "src/colors.hpp"
#include "parser.tab.hpp"

int line_counter = 1; /* Counter for lines in original file, used for reporting errors */

/* Global variables used for storing text lines */
int text_line_counter = 0;
int line_characters_counter = 0;
std::vector<std::string> file_text{""};
std::map<int, int> added;

/* Global variables used for if text line buffer */
int if_text_line_counter = 0;
int if_line_characters_counter = 0;
std::vector<std::string> if_file_text{""};

bool if_active = false; /* This flag tells if lines should be inserted in if buffer or in regular text buffer */
%}

NUM [0-9]
%%
"{%" {
    BEGIN(code);    /* Opens separate parsing type for code part */
    return begin_token;
}
<code>"%}" {
    BEGIN(INITIAL);     /* Closes separate parsing type for code part */
    return end_token;
}
<code>"true"|"True" return true_token;
<code>"false"|"False" return false_token;
<code>#[^\n]* {}    /* Code comments are ignored */
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
    s = s.substr(0, s.size() - 1);      /* String is declared between two " characters, so substring without them is taken */
    yylval.str_type = new std::string(s);
    return string_token;
}
<code>[;\[\]=:{},#] return yytext[0];
<code>\n {
    line_counter++;     /* Line counter from original file increments in every parsing type */
}
<code>[\t ] { }     /* Tabs and spaces are being ignored */
<code>. {
    /* Any unrecognised character will cause an error, which will be colored in green */
    std::cout << Color::set_green("Line " + std::to_string(line_counter) + ":Error - Unknown character: " + *yytext) << std::endl;  
    exit(EXIT_FAILURE);
}


"{{" {
    BEGIN(code_text);       /* Opens separate parsing type for text part */
    return begin_text_token;
}
<code_text>"}}" {
    BEGIN(INITIAL);         /* Closes separate parsing type for text statement part */
    return end_text_token;
}
<code_text>"if" {
    BEGIN(if_stat);         /* Opens separate parsing type for if statement, because in that part only id tokens should be recognised */
    if_active = true;       /* Activates flag for inserting in if buffer */
    
    /* Every time if statement is recognised, if buffer is cleared */
    if_file_text.clear();   
    if_file_text.push_back("");
    if_text_line_counter = 0;
    if_line_characters_counter = 0;
    
    return if_token;
}
<code_text>[0-9]+ {
    yylval.int_type = atoi(yytext);
    return int_num_token;
}
<code_text>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}
<code_text>[\[\].] return yytext[0];
<code_text>[ \t] {}     /* Tabs and spaces are being ignored */
<code_text>. {
    /* Any unrecognised character will cause an error, which will be colored in green */
    std::cout << Color::set_green("Line " + std::to_string(line_counter) + ":Error - Unknown character: " + *yytext) << std::endl;  
    exit(EXIT_FAILURE);
}

<if_stat>"}}" {
    BEGIN(if_text);         /* Opens separate parsing type for if text part, beacuse there is text that should be ignored */
    return end_text_token;
}
<if_stat>"endif" {
    BEGIN(code_text);       /* Opens separate parsing type for if statement part, and returns to parsing for code text */
    
    if_active = false;      /* Deactivates flag for writing in if buffer */
    return endif_token;
}
<if_stat>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}
<if_stat>[ \t] {}   /* Tabs and spaces are being ignored */
<if_stat>. {
    /* Any unrecognised character will cause an error, which will be colored in green */
    std::cout << Color::set_green("Line " + std::to_string(line_counter) + ":Error - Unknown character: " + *yytext) << std::endl;  
    exit(EXIT_FAILURE);
}

<if_text>"{{" { 
    BEGIN(if_stat);         /* Opens separate parsing type for if statement part */
    return begin_text_token;
}
<if_text>"}}" {
    BEGIN(if_text);         /* Opens separate parsing type for if text part */
    return end_text_token;
}
<if_text>\n {
    line_counter++;
        
    if_file_text[if_text_line_counter].append(yytext);
    if_text_line_counter++;
    if_line_characters_counter = 0;
    
    if_file_text.push_back("");
    
}
<if_text>. {
    if_file_text[if_text_line_counter].append(yytext);
    if_line_characters_counter++;
}

\n {
    line_counter++;
    
    if(text_line_counter == 0)
        added[0] = 0;
    
    file_text[text_line_counter].append(yytext);
    text_line_counter++;
    line_characters_counter = 0;
    
    file_text.push_back("");
    added[text_line_counter] = 0;
}

. {
    file_text[text_line_counter].append(yytext);
    line_characters_counter++;
}

%%
