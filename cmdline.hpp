//
//  cmdline.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#ifndef cmdline_hpp
#define cmdline_hpp

#include <stdio.h>
#include <string>

void use_arguments(int argc, const char * argv[]);

// Expr Class
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
};
// cmdline.hpp
// Num Class
class Num : public Expr {
public:
    int val;
    
    // constructor
    Num(int val);
    
    bool equals(Expr *o);
};

// Add Class
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Add(Expr *lhs, Expr *rhs);
};

// Mult Class
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Mult(Expr *lhs, Expr *rhs);
};

// Variable Class
class Variable : public Expr {
public:
    std::string string;
    
    // constructor
    Variable(std::string string);
};

#endif /* cmdline_hpp */


