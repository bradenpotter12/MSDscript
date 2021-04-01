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
#include "pointer.h"

class Expr;

class Val : public std::enable_shared_from_this<Val> {
public:
    virtual bool equals(PTR(Val) o) = 0;
    //virtual int interp() = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual PTR(Val) add_to(PTR(Val) rhs) = 0;
    virtual PTR(Val) mult_to(PTR(Val) rhs) = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual std::string to_string() = 0;
};

class NumVal : public Val {
public:
    
    int val;
    
    // constructor
    NumVal(int val);
    
    bool equals(PTR(Val) o);
    int interp();
    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) rhs);
    PTR(Val) mult_to(PTR(Val) rhs);
    std::string to_string();
    void print(std::ostream &out);
    PTR(Val) call(PTR(Val) actual_arg);
};

class BoolVal : public Val {
public:
    
    bool value;
    
    BoolVal(bool value);
    
    bool equals(PTR(Val) o);
    int interp();
    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) rhs);
    PTR(Val) mult_to(PTR(Val) rhs);
    std::string to_string();
    void print(std::ostream &out);
    PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val {
public:
    
    std::string formal_arg;
    PTR(Expr) body;
    
    FunVal(std::string formal_arg, PTR(Expr) body);
    
    bool equals(PTR(Val) o);
    PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) rhs);
    PTR(Val) mult_to(PTR(Val) rhs);
    std::string to_string();
    void print(std::ostream &out);
    PTR(Val) call(PTR(Val) actual_arg);
};

#endif /* val_hpp */
