#ifndef __VARIABLES_HPP__
#define __VARIABLES_HPP__ 1

#include <string>
#include <vector>

class Variable {
public:
    Variable() = default;

    ~Variable() = default;
};

class String : public Variable {
public:
    String(std::string value)
    : _value(value)
    {}

private:
    std::string _value;
};

class Number : public Variable {
public:
    Number(float value)
    : _value(value)
    {}

private:
    float _value;
};

class Boolean : public Variable {
public:
    Boolean(bool value)
    : _value(value)
    {}

private:
    bool _value;
};

class Object {
public:
    Object(std::vector<Variable> fields)
    : _fields(fields)
    {}

private:
    std::vector<Variable> _fields;
};

#endif