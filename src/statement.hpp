#ifndef __STATEMENT_HPP__ 
#define __STATEMENT_HPP__ 1

#include "variables.hpp"
#include "colors.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

class Statement {
public:
    Statement() = default;

    virtual void run() const = 0;
    virtual ~Statement() = default;
};

class Block : public Statement {
public:
    Block(std::vector<Statement*> statements)
    : _statements(statements)
    {}

    void run() const override;
    ~Block();
private:
    std::vector<Statement*> _statements;
    Block(const Block& b);
    Block& operator=(const Block& b);
};

class Assignment : public Statement {
public:
    Assignment(std::string id, Variable* value)
    : _id(id), _value(value)
    {}

    void run() const override;
    ~Assignment();
private:
    std::string _id;
    Variable* _value;
    Assignment(const Assignment& a);
    Assignment& operator=(const Assignment& a);
};

class ObjectTextStatement : public Statement {
public:
    ObjectTextStatement(std::string object, std::string atribute, int line_number = 0, int character_place = 0)
    : _object(object), _atribute(atribute), _line_number(line_number), _character_place(character_place)
    {}
    
    void run() const override;
    void set_line_number(int value);
    void set_character_place(int value);
private:
    std::string _object;
    std::string _atribute;
    int _line_number;
    int _character_place;
};

class ArrayTextStatement : public Statement {
public: 
    ArrayTextStatement(std::string name, int index, int line_number = 0, int character_place = 0)
    : _name(name), _index(index), _line_number(line_number), _character_place(character_place)
    {}
    
    void run() const override;
    void set_line_number(int value);
    void set_character_place(int value);
private:
    std::string _name;
    int _index;
    int _line_number;
    int _character_place;
};

class VarTextStatement : public Statement {
public: 
    VarTextStatement(std::string name, int line_number = 0, int character_place = 0)
    : _name(name), _line_number(line_number), _character_place(character_place)
    {}
    
    void run() const override;
    void set_line_number(int value);
    void set_character_place(int value);
private:
    std::string _name;
    int _line_number;
    int _character_place;
};

class IfStatement : public Statement {
public:
    IfStatement(std::string id, std::vector<Statement*> statements, int place, std::vector<std::string> text)
    : _id(id), _statements(statements), _place(place), _text(text)
    {}
    
    void run() const override;
private:
    std::string _id;
    std::vector<Statement*> _statements;
    int _place;
    std::vector<std::string> _text;
};

class ForStatement : public Statement {
public:
    ForStatement(std::string id, std::string in, std::vector<Statement*> statements, int place, std::vector<std::string> text)
    : _id(id), _in(in), _statements(statements), _place(place), _text(text)
    {}
    
    void run() const override;
private:
    std::string _id;
    std::string _in;
    std::vector<Statement*> _statements;
    int _place;
    std::vector<std::string> _text;
};

#endif
