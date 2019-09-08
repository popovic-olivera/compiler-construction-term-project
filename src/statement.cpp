#include "statement.hpp"

extern std::map<std::string, Variable*> variables_table;
extern void yyerror(std::string msg);
extern std::vector<std::string> file_text;
extern std::map<int, int> added;
extern int line_counter;

void Block::run() const
{
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
    variables_table[_id] = _value;
}

Assignment::~Assignment()
{
    delete _value;
}

void ObjectTextStatement::run() const
{
    auto finder = variables_table.find(_object);
    
    if(finder != variables_table.end())
    {
        Variable* o = finder->second;
        
        if(o->get_type() != OBJ)
            yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - '" + _object + "' is not an object."));
        
        Object* obj = (Object*)o;
        
        auto map = obj->get_atributes();
        
        auto finder2 = map.find(_atribute);
        
        if(finder2 != map.end())
        {
            std::string value = finder2->second->print();
            file_text[_line_number] = file_text[_line_number].insert(_character_place + added[_line_number], value);
            added[_line_number] += value.size();
        }
        else
        {
            yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - Not found. Atribute: '" + _atribute + "' of object: '" + _object + "' does not exist."));
        }
    }
    else 
    {
        yyerror(Color::set_blue("Line " + std::to_string(line_counter) + ":Error - Not found. Object: '" + _object + "' does not exist."));
    }
}

void ObjectTextStatement::set_line_number(int value)
{
    _line_number = value;
}

void ObjectTextStatement::set_character_place(int value)
{
    _character_place = value;
}

void ArrayTextStatement::run() const
{
    auto finder = variables_table.find(_name);
    
    if(finder != variables_table.end())
    {
        Variable* a = finder->second;
        
        if(a->get_type() != ARR)
            yyerror(Color::set_magenta("Line " + std::to_string(line_counter) + ":Error - '" + _name + "' is not an array."));
        
        Array* arr = (Array*)a;
        
        auto array = arr->get_value();
        
        std::string value = array[_index]->print();
        file_text[_line_number] = file_text[_line_number].insert(_character_place + added[_line_number], value);
        added[_line_number] += value.size();
    }
    else 
    {
        yyerror(Color::set_magenta("Line " + std::to_string(line_counter) + ":Error - Not found. Array: '" + _name + "' does not exist."));
    }
}

void ArrayTextStatement::set_line_number(int value)
{
    _line_number = value;
}

void ArrayTextStatement::set_character_place(int value)
{
    _character_place = value;
}

void VarTextStatement::run() const
{
    auto finder = variables_table.find(_name);
    
    if(finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if(v->get_type() == ARR || v->get_type() == OBJ)
            yyerror(Color::set_cyan("Line " + std::to_string(line_counter) + ":Error - '" + _name + "' is not a variable."));
        
        std::string value = v->print();
        file_text[_line_number] = file_text[_line_number].insert(_character_place + added[_line_number], value);
        added[_line_number] += value.size();
    }
    else 
    {
        yyerror(Color::set_cyan("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _name + "' does not exist."));
    }
}

void VarTextStatement::set_line_number(int value)
{
    _line_number = value;
}

void VarTextStatement::set_character_place(int value)
{
    _character_place = value;
}

void IfStatement::run() const
{
    // TODO 
}
