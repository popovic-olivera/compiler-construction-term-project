#ifndef __VARIABLES_HPP__
#define __VARIABLES_HPP__ 1

#include <string>
#include <vector>
#include <map>
#include <iostream>

class Variable {
public:
    Variable() = default;

    ~Variable() = default;
    virtual void print() const = 0;
};

class String : public Variable {
public:
    String(std::string value)
    : _value(value)
    {}

    void print() const override;
private:
    std::string _value;
};

class Number : public Variable {
public:
    Number(float value)
    : _value(value)
    {}

    void print() const override;
private:
    float _value;
};

class Boolean : public Variable {
public:
    Boolean(bool value)
    : _value(value)
    {}

    void print() const override;
private:
    bool _value;
};

class Array : public Variable {
public:
    Array(std::vector<Variable*> value)
    : _value(value)
    {}

    void print() const override;
private:
    std::vector<Variable*> _value;
};

class Object : public Variable {
public:
    Object(std::map<std::string, Variable*> atributes)
    : _atributes(atributes)
    {}

    void print() const override;
private:
    std::map<std::string, Variable*> _atributes;
};

#endif