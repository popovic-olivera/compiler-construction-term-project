#include "variables.hpp"

void String::print() const
{
    std::cout << _value << std::endl;
}

void Number::print() const 
{
    std::cout << _value << std::endl;
}

void Boolean::print() const
{
    std::cout << _value << std::endl;
}

void Array::print() const
{
    auto it = _value.begin();
    while(it != _value.end())
    {
        (*it)->print(); 

        it++;
    }
}

void Object::print() const
{
    auto it = _atributes.begin();
    while(it != _atributes.end())
    {
        std::cout << it->first << " = ";
        it->second->print();

        it++;
    }
}