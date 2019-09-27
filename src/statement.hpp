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

    virtual void run() const = 0;       /* This virtual method will be used for runing every statement */
    virtual ~Statement() = default;     /* Virtual destructor has to be made because class has virtual method*/
};

class Block : public Statement {
public:
    Block(std::vector<Statement*> statements)           /* Block of code has an array of statements */
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
    Assignment(std::string id, Variable* value)         /* Assignment statement consists of 'id = value' */
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

class IfStatement : public Statement {
public:
    /* This statement consists of '{{ if id }} ... {{ endif }}', where ... is array of statements.
       Place is needed to know in which line to insert text, after it has been processed. */
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
    /* This statement consists of '{{ for id1 in id2 }} ... {{ endfor }}', where ... is array of statements.
       Place is needed to know in which line to insert text, after it has been processed. */
    ForStatement(std::string id1, std::string id2, std::vector<Statement*> statements, int place, std::vector<std::string> text)
    : _id1(id1), _id2(id2), _statements(statements), _place(place), _text(text)
    {}
    
    void run() const override;
private:
    std::string _id1;
    std::string _id2;
    std::vector<Statement*> _statements;
    int _place;
    std::vector<std::string> _text;
};

class TextStatement : public Statement {
public: 
    TextStatement(std::string name, int line_number, int character_place)
    : _name(name), _line_number(line_number), _character_place(character_place)
    {}
    
    void set_line_number(int value);
    void set_character_place(int value);
protected:
    std::string _name;
    int _line_number;
    int _character_place;
};

class ObjectTextStatement : public TextStatement {
public:
    /* This statement consists of 'id.id'. Line number and character_place are needed for knowing in which text/buffer line and where in that line
     * to write result to */
    ObjectTextStatement(std::string name, std::string atribute, int line_number = 0, int character_place = 0)
    : TextStatement(name, line_number, character_place), _atribute(atribute)
    {}
    
    void run() const override;
private:
    std::string _atribute;
};

class ArrayTextStatement : public TextStatement {
public: 
    /* This statement consists of 'id[index]'. Line number and character_place are needed for knowing in which text/buffer line and where in that line
     * to write result to */
    ArrayTextStatement(std::string name, int index, int line_number = 0, int character_place = 0)
    : TextStatement(name, line_number, character_place), _index(index)
    {}
    
    void run() const override;
private:
    int _index;
};

class VarTextStatement : public TextStatement {
public: 
    /* This statement consists of 'id'. Line number and character_place are needed for knowing in which text/buffer line and where in that line
     * to write result to */
    VarTextStatement(std::string name, int line_number = 0, int character_place = 0)
    : TextStatement(name, line_number, character_place)
    {}
    
    void run() const override;
};

#endif
