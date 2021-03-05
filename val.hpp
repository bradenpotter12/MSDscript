//
//  val.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/3/21.
//

#ifndef val_hpp
#define val_hpp

#include <stdio.h>

class Expr;

class Val {
public:
    virtual bool equals(Val *o) = 0;
    virtual int interp() = 0;
    virtual Expr* to_expr() = 0;
    virtual Val* add_to(Val *rhs) = 0;
    virtual Val* mult_to(Val *rhs) = 0;
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
};

#endif /* val_hpp */
