#include "statement.hpp"

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

extern std::map<std::string, Variable*> variables_table;

void Assignment::run() const
{
    variables_table[_id] = _value;
}

Assignment::~Assignment()
{
    delete _value;
}