#include "variables.hpp"

std::string String::print() const
{
    return _value;
}

std::string Number::print() const 
{
    return std::to_string(_value);
}

std::string Boolean::print() const
{
    if(_value)
        return "true";
    else
        return "false";
}

std::string Array::print() const
{
    return "";
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
