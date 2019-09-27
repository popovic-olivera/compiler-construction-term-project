#include "statement.hpp"

extern void yyerror(std::string msg);   /* Function for reporting errors defined in parser.ypp */

extern std::map<std::string, Variable*> variables_table;    /* Map for storing variables defined in parser.ypp */

/* Global variables used for saving text in file which has to be changed, declared in lexer.lex */
extern std::vector<std::string> file_text;
extern std::map<int, int> added;

/* This global variable is used for reporting errors */
extern int line_counter;

/* Variables used for switching between writing into text buffer and if/for help buffer */
bool if_activated = false;
bool for_activated = false;

/* Help variables for if/for buffers - counting how many lines 
 * and how many characters in each line have been added */
std::vector<std::string> buffer;
std::map<int, int> buffer_added;
int added_with_buffers = 0;

void Block::run() const
{
    /* For Block, just go through statements array and run every one */
    for(unsigned i = 0; i < _statements.size(); i++)
        _statements[i]->run();
}

Block::~Block()
{
    for(unsigned i = 0; i < _statements.size(); i++)
        delete _statements[i];
}

void Assignment::run() const
{
    /* Assign value to given id in variables table */
    variables_table[_id] = _value;
}

Assignment::~Assignment()
{
    delete _value;
}

void TextStatement::set_line_number(int value)
{
    _line_number = value;
}

void TextStatement::set_character_place(int value)
{
    _character_place = value;
}

/* Errors with if and for statements are colored in yellow :) */
void IfStatement::run() const
{
    buffer = _text;     /* Get saved text between {{if id}} and {{endif}} */
 
    /* Reset map for added characters in each line */
    for(auto it = buffer_added.begin(); it != buffer_added.end(); it++)
        it->second = 0;
    
    auto finder = variables_table.find(_id);    /* Try to find variable _id */
    
    if (finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if (v->get_type() != BOOL)
            yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - " + _id + " is not a variable"));

        std::string value = v->print();
        if (value == "true")
        {
            if_activated = true;    /* Activate flag for writing into if buffer */

            for(auto s : _statements)
                s->run();
            
            /* Remove empty lines before writing into file text */
            for(unsigned i = 0; i < buffer.size(); i++)
            {
                if (buffer[i] == "\n")
                {
                    auto it = buffer.begin();
                    buffer.erase(it + i);
                }
                if (buffer[i].size() == 0)
                {
                    auto it = buffer.begin();
                    buffer.erase(it + i);
                }
            }
            
            /* Insert content of if buffer to file text */
            file_text.insert(file_text.begin() + _place + added_with_buffers, buffer.begin(), buffer.end());
            added_with_buffers += buffer.size();
            
            if_activated = false;   /* Deactivate flag for writing into if buffer */
        }
    }
    else 
    {
        yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _id + "' does not exist."));
    }
    
    buffer.clear();
}

/* Errors with if and for statements are colored in yellow :) */
void ForStatement::run() const
{
    buffer = _text;     /* Get saved text between {{for id in id}} and {{endfor}}*/
    
    /* Reset map for added characters in each line */
    for(auto it = buffer_added.begin(); it != buffer_added.end(); it++)
        it->second = 0;
    
    auto finder = variables_table.find(_id2);    /* Try to find variable _id1 */
    
    if (finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if (v->get_type() != ARR)
            yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - " + _id1 + " is not array."));

        for_activated = true;   /* Activate flag for writing into for buffer */
        
        std::vector<Variable*> array = ((Array*)v)->get_value();    /* Get array elements */
        
        for(auto& a : array)
        {
            variables_table[_id1] = a;
            
            for(auto s : _statements)
                s->run();
        }
            
        /* Remove empty lines before writing into file text */
        for(unsigned i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] == "\n")
            {
                auto it = buffer.begin();
                buffer.erase(it + i);
            }
            if (buffer[i].size() == 0)
            {
                auto it = buffer.begin();
                buffer.erase(it + i);
            }
        }
            
        /* Insert content of for buffer to file text */
        file_text.insert(file_text.begin() + _place + added_with_buffers, buffer.begin(), buffer.end());
        added_with_buffers += buffer.size();
            
        for_activated = false;      /* Deactivate flag for writing into for buffer */
    }
    else 
    {
        yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _id1 + "' does not exist."));
    }
    
    buffer.clear();
}

/* Errors with objects are colored in blue :) */
void ObjectTextStatement::run() const
{
    auto finder = variables_table.find(_name);    /* Try to find _name */
    
    if (finder != variables_table.end())
    {
        Variable* o = finder->second;
        
        if (o->get_type() != OBJ)
            yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - '" + _name + "' is not an object."));
        
        Object* obj = (Object*)o;
        
        /* Get list of atributes and try to find desired one */
        auto map = obj->get_atributes();
        auto finder2 = map.find(_atribute);
        
        if (finder2 != map.end())
        {
            std::string value = "";
            if (for_activated)   /* If in for buffer, add space after every word */
                value = finder2->second->print() + " ";
            else
                value = finder2->second->print();
            
            /* If writing to if/for buffer is activated, write into it, else write into file text */
            if (if_activated || for_activated)
            {
                /* + buffer_added[_line_number] is added because every time something is added into line with _line_number,
                     character place of current result increases */
                buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
                buffer_added[_line_number] += value.size();     /* Remember how many characters have been added in this step  */
            }
            else
            {
                int l = _line_number + added_with_buffers;  /* This is added because every time buffer is inserted into file text it changes line number */
                
                /* + added[_line_number] is added because every time something is added into line with _line_number,
                     character place of current result increases */
                file_text[l] = file_text[l].insert(_character_place + added[_line_number], value);
                added[_line_number] += value.size();        /* Add in map how much has been added in this step for _line_number line */
            }
        }
        else
        {
            yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - Not found. Atribute: '" + _atribute + "' of object: '" + _name + "' does not exist."));
        }
    }
    else 
    {
        yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - Not found. Object: '" + _name + "' does not exist."));
    }
}

/* Errors with arrays are colored in magenta :) */
void ArrayTextStatement::run() const
{
    auto finder = variables_table.find(_name);  /* Try to find _name */
    
    if (finder != variables_table.end())
    {
        Variable* a = finder->second;
        
        if (a->get_type() != ARR)
            yyerror(Color::set_magenta("Line " + std::to_string(line_counter) + ":Error - '" + _name + "' is not an array."));
        
        Array* arr = (Array*)a;
        
        /* Get array elements and take value of one with index equal to _index */
        auto array = arr->get_value();
        
        std::string value = "";
        if (for_activated)   /* If in for buffer, add space after every word */
            value = array[_index]->print() + " ";
        else
            value = array[_index]->print();
        
        /* If writing to if/for buffer is activated, write into it, else write into file text */
        if (if_activated || for_activated)
        {
            /* + buffer_added[_line_number] is added because every time something is added into line with _line_number,
                 character place of current result increases */
            buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
            buffer_added[_line_number] += value.size();     /* Remember how many characters have been added in this step  */
        }
        else
        {
            int l = _line_number + added_with_buffers;  /* This is added because every time buffer is inserted into file text it changes line number */
            
            /* + added[_line_number] is added because every time something is added into line with _line_number,
                 character place of current result increases */
            file_text[l] = file_text[l].insert(_character_place + added[_line_number], value);
            added[_line_number] += value.size();        /* Add in map how much has been added in this step for _line_number line */
        }
    }
    else 
    {
        yyerror(Color::set_magenta("Line " + std::to_string(line_counter) + ":Error - Not found. Array: '" + _name + "' does not exist."));
    }
}

/* Errors with variables are colored in cyan :) */
void VarTextStatement::run() const
{
    auto finder = variables_table.find(_name);      /* Try to find _name */
    
    if (finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if (v->get_type() == ARR || v->get_type() == OBJ)
            yyerror(Color::set_cyan("Line " + std::to_string(line_counter) + ":Error - '" + _name + "' is not a variable."));
        
        std::string value = "";
        if (for_activated)   /* If in for buffer, add space after every word */
            value = v->print() + " ";
        else
            value = v->print();
        
        /* If writing to if/for buffer is activated, write into it, else write into file text */
        if ( if_activated or for_activated)
        {
            /* + buffer_added[_line_number] is added because every time something is added into line with _line_number,
                 character place of current result increases */
            buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
            buffer_added[_line_number] += value.size();     /* Remember how many characters have been added in this step  */
        }
        else
        {
            int l = _line_number + added_with_buffers;    /* This is added because every time buffer is inserted into file text it changes line number */
            
            /* + added[_line_number] is added because every time something is added into line with _line_number,
                 character place of current result increases */
            file_text[l] = file_text[l].insert(_character_place + added[_line_number], value);
            added[_line_number] += value.size();         /* Add in map how much has been added in this step for _line_number line */
        }
    }
    else 
    {
        yyerror(Color::set_cyan("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _name + "' does not exist."));
    }
}
