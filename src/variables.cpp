#include "variables.hpp"

std::string String::print() const
{
    return _value;
}

Type String::get_type() const
{
    return STR;
}

std::string Number::print() const 
{
    std::string value = std::to_string(_value);
    std::size_t dot_place = value.find(".");
    value = value.substr(0, dot_place + 3);
    return value;
}

Type Number::get_type() const
{
    return NUM;
}

std::string Boolean::print() const
{
    if(_value)
        return "true";
    else
        return "false";
}

Type Boolean::get_type() const
{
    return BOOL;
}

std::string Array::print() const
{
    return "";
}

Type Array::get_type() const
{
    return ARR;
}

std::vector<Variable*> Array::get_value() const
{
    return _value;
}

Array::~Array()
{
    for(unsigned i = 0; i < _value.size(); i++)
        delete _value[i];
}

std::string Object::print() const
{
    return "";
}

Type Object::get_type() const
{
    return OBJ;
}

std::map<std::string, Variable*> Object::get_atributes() const
{
    return _atributes;
}

Object::~Object()
{
    auto it = _atributes.begin();
    while(it != _atributes.end())
    {
        delete it->second;

        it++;
    }

    _atributes.clear();
}
