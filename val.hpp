//
//  val.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/3/21.
//

#ifndef val_hpp
#define val_hpp

#include <stdio.h>
#include <string>

class Expr;

class Val {
public:
    virtual bool equals(Val *o) = 0;
    //virtual int interp() = 0;
    virtual Expr* to_expr() = 0;
    virtual Val* add_to(Val *rhs) = 0;
    virtual Val* mult_to(Val *rhs) = 0;
    virtual Val* call(Val *actual_arg) = 0;
    virtual std::string to_string() = 0;
};

class NumVal : public Val {
public:
    
    int val;
    
    // constructor
    NumVal(int val);
    
    bool equals(Val *o);
    int interp();
    Expr* to_expr();
    Val* add_to(Val *rhs);
    Val* mult_to(Val *rhs);
    std::string to_string();
    void print(std::ostream &out);
    Val* call(Val *actual_arg);
};

class BoolVal : public Val {
public:
    
    bool value;
    
    BoolVal(bool value);
    
    bool equals(Val *o);
    int interp();
    Expr* to_expr();
    Val* add_to(Val *rhs);
    Val* mult_to(Val *rhs);
    std::string to_string();
    void print(std::ostream &out);
    Val* call(Val *actual_arg);
};

class FunVal : public Val {
public:
    
    std::string formal_arg;
    Expr *body;
    
    FunVal(std::string formal_arg, Expr *body);
    
    bool equals(Val *o);
    int interp();
    Expr* to_expr();
    Val* add_to(Val *rhs);
    Val* mult_to(Val *rhs);
    std::string to_string();
    void print(std::ostream &out);
    Val* call(Val *actual_arg);
};

#endif /* val_hpp */
