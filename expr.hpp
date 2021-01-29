#include <string>
#include <stdio.h>

// Expr Class
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp(Expr *e) = 0;
};

// Num Class
class Num : public Expr {
public:
    int val;
    
    // constructor
    Num(int val);
    
    bool equals(Expr *o);
    int interp(Expr *o);
};

// Add Class
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Add(Num *lhs, Num *rhs);
    
    bool equals(Expr *o);
    int interp(Expr *o);
};

// Mult Class
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Mult(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    int interp(Expr *o);
};

// Variable Class
class Variable : public Expr {
public:
    std::string string;
    
    // constructor
    Variable(std::string string);
    
    bool equals(Expr *o);
    int interp(Expr *o);
};
