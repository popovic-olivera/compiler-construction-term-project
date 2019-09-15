#include "statement.hpp"

extern std::map<std::string, Variable*> variables_table;
extern void yyerror(std::string msg);
extern std::vector<std::string> file_text;
extern std::map<int, int> added;
extern int line_counter;
bool if_activated = false;
bool for_activated = false;
std::map<int, int> buffer_added;

int added_with_if = 0;
std::vector<std::string> buffer;

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
            
            if( if_activated)
            {
                buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
                buffer_added[_line_number] += value.size();
            }
            else
            {
                int _line_num = _line_number + added_with_if;
                file_text[_line_num] = file_text[_line_num].insert(_character_place + added[_line_number], value);
                added[_line_number] += value.size();
            }
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
        
        if( if_activated)
        {
            buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
            buffer_added[_line_number] += value.size();
        }
        else
        {
            int _line_num = _line_number + added_with_if;
            file_text[_line_num] = file_text[_line_num].insert(_character_place + added[_line_number], value);
            added[_line_number] += value.size();
        }
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
        
        std::string value = "";
        if(for_activated)
            value = v->print() + " ";
        else
            value = v->print();
        
        if( if_activated or for_activated)
        {
            buffer[_line_number] = buffer[_line_number].insert(_character_place + buffer_added[_line_number], value);
            buffer_added[_line_number] += value.size(); 
        }
        else
        {
            int _line_num = _line_number + added_with_if;
            file_text[_line_num] = file_text[_line_num].insert(_character_place + added[_line_number], value);
            added[_line_number] += value.size(); 
        }
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
    buffer = _text;
 
    for(auto it = buffer_added.begin(); it != buffer_added.end(); it++)
        it->second = 0;
    
    auto finder = variables_table.find(_id);
    
    if(finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if(v->get_type() != BOOL)
            yyerror(Color::set_yellow("In if statement must be a bool variable. Error - " + _id + " is not a variable"));

        std::string value = v->print();
        if(value == "true")
        {
            if_activated = true;

            for(auto s : _statements)
                s->run();
            
            for(unsigned i = 0; i < buffer.size(); i++)
            {
                if(buffer[i] == "\n")
                {
                    auto it = buffer.begin();
                    buffer.erase(it + i);
                }
                if(buffer[i].size() == 0)
                {
                    auto it = buffer.begin();
                    buffer.erase(it + i);
                }
            }
            
            file_text.insert(file_text.begin() + _place + added_with_if, buffer.begin(), buffer.end());
            added_with_if += buffer.size();
            
            if_activated = false;
        }
    }
    else 
    {
        yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _id + "' does not exist."));
    }
}

void ForStatement::run() const
{
    buffer = _text;
    
    for(auto it = buffer_added.begin(); it != buffer_added.end(); it++)
        it->second = 0;
    
    auto finder = variables_table.find(_in);
    
    if(finder != variables_table.end())
    {
        Variable* v = finder->second; 
        
        if(v->get_type() != ARR)
            yyerror(Color::set_yellow("In for statement must be an array. Error - " + _id + " is not array."));

        for_activated = true;
        
        std::vector<Variable*> array = ((Array*)v)->get_value();
        
        for(auto& a : array)
        {
            variables_table[_id] = a;
            
            for(auto s : _statements)
                s->run();
        }
            
        for(unsigned i = 0; i < buffer.size(); i++)
        {
            if(buffer[i] == "\n")
            {
                auto it = buffer.begin();
                buffer.erase(it + i);
            }
            if(buffer[i].size() == 0)
            {
                auto it = buffer.begin();
                buffer.erase(it + i);
            }
        }
            
        file_text.insert(file_text.begin() + _place + added_with_if, buffer.begin(), buffer.end());
        added_with_if += buffer.size();
            
        for_activated = false;
    }
    else 
    {
        yyerror(Color::set_yellow("Line " + std::to_string(line_counter) + ":Error - Not found. Variable: '" + _id + "' does not exist."));
    }
}
