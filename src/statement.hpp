#ifndef __STATEMENT_HPP__ 
#define __STATEMENT_HPP__ 1

#include "variables.hpp"
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

#endif