#include <string>
#include <stdio.h>

// Expr Class
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    
    // Returns an int for the value of an expression
    virtual int interp() = 0;
    
    // Returns true if the expression is a variable or contains
    // a variable, false otherwise
    virtual bool has_variable() = 0;
};

// Num Class
class Num : public Expr {
public:
    int val;
    
    // constructor
    Num(int val);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
};

// Add Class
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Add(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
};

// Mult Class
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Mult(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
};

// Variable Class
class Variable : public Expr {
public:
    std::string string;
    
    // constructor
    Variable(std::string string);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
};
