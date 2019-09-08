#ifndef __VARIABLES_HPP__
#define __VARIABLES_HPP__ 1

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "colors.hpp"

enum Type {
    STR,
    NUM,
    BOOL,
    ARR,
    OBJ
};

class Variable {
public:
    Variable() = default;

    virtual ~Variable() = default;
    virtual std::string print() const = 0;
    virtual Type get_type() const = 0;
};

class String : public Variable {
public:
    String(std::string value)
    : _value(value)
    {}

    std::string print() const override;
    Type get_type() const override;
private:
    std::string _value;
};

class Number : public Variable {
public:
    Number(float value)
    : _value(value)
    {}

    std::string print() const override;
    Type get_type() const override;
private:
    float _value;
};

class Boolean : public Variable {
public:
    Boolean(bool value)
    : _value(value)
    {}

    std::string print() const override;
    Type get_type() const override;
private:
    bool _value;
};

class Array : public Variable {
public:
    Array(std::vector<Variable*> value)
    : _value(value)
    {}

    ~Array();
    std::string print() const override;
    std::vector<Variable*> get_value() const;
    Type get_type() const override;
private:
    std::vector<Variable*> _value;
    Array(const Array& a);
    Array& operator=(const Array& a);
};

class Object : public Variable {
public:
    Object(std::map<std::string, Variable*> atributes)
    : _atributes(atributes)
    {}

    ~Object();
    std::string print() const override;
    std::map<std::string, Variable*> get_atributes() const;
    Type get_type() const override;
private:
    std::map<std::string, Variable*> _atributes;
    Object(const Object& o);
    Object& operator=(const Object& o);
};

#endif
