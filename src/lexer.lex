%option nounput
%option noinput
%option noyywrap

%x code
%x code_text
%x buffer_text
%x statement

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
int buffer_text_line_counter = 0;
int buffer_line_characters_counter = 0;
std::vector<std::string> buffer_file_text{""};

bool active = false; /* This flag tells if lines should be inserted in if buffer or in regular text buffer */
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
    BEGIN(statement);         /* Opens separate parsing type for if statement, because in that part only id tokens should be recognised */
    active = true;       /* Activates flag for inserting in if buffer */
    
    /* Every time if statement is recognised, if buffer is cleared */
    buffer_file_text.clear();   
    buffer_file_text.push_back("");
    buffer_text_line_counter = 0;
    buffer_line_characters_counter = 0;
    
    return if_token;
}
<code_text>"for" {
    BEGIN(statement);
    active = true;
    
    buffer_file_text.clear();   
    buffer_file_text.push_back("");
    buffer_text_line_counter = 0;
    buffer_line_characters_counter = 0;
    
    return for_token;
}
<code_text>[1-9]{NUM}*|{NUM}+(\.{NUM}+)? {
    yylval.num_type = atof(yytext);
    return num_token;
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

<statement>"}}" {
    BEGIN(buffer_text);         /* Opens separate parsing type for if text part, beacuse there is text that should be ignored */
    return end_text_token;
}
<statement>"endif" {
    BEGIN(code_text);       /* Opens separate parsing type for if statement part, and returns to parsing for code text */
    
    active = false;      /* Deactivates flag for writing in if buffer */
    return endif_token;
}
<statement>"endfor" {
    BEGIN(code_text);
    return endfor_token;
}
<statement>"in" return in_token;
<statement>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_type = new std::string(yytext);
    return id_token;
}
<statement>[ \t] {}   /* Tabs and spaces are being ignored */
<statement>[.\[\]] return yytext[0];
<statement>[1-9]{NUM}*|{NUM}+(\.{NUM}+)? {
    yylval.num_type = atof(yytext);
    return num_token;
}
<statement>. {
    /* Any unrecognised character will cause an error, which will be colored in green */
    std::cout << Color::set_green("Line " + std::to_string(line_counter) + ":Error - Unknown character: " + *yytext) << std::endl;  
    exit(EXIT_FAILURE);
}

<buffer_text>"{{" { 
    BEGIN(statement);         /* Opens separate parsing type for if statement part */
    return begin_text_token;
}
<buffer_text>"}}" {
    BEGIN(buffer_text);         /* Opens separate parsing type for if text part */
    return end_text_token;
}
<buffer_text>\n {
    line_counter++;
        
    buffer_file_text[buffer_text_line_counter].append(yytext);
    buffer_text_line_counter++;
    buffer_line_characters_counter = 0;
    
    buffer_file_text.push_back("");
}
<buffer_text>. {
    buffer_file_text[buffer_text_line_counter].append(yytext);
    buffer_line_characters_counter++;
}

\n {
    line_counter++;
    
    if (text_line_counter == 0)
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
