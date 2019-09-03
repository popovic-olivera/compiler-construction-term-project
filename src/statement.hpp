#ifndef __STATEMENT_HPP__ 
#define __STATEMENT_HPP__ 1

class Statement {
public:
    virtual void run() const = 0;

    ~Statement() = default;
};

#endif