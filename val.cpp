//
//  val.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/3/21.
//

#include "val.hpp"
#include "expr.hpp"
#include "catch.hpp"


// constructor
NumVal::NumVal(int val) {
    this->val = val;
}

bool NumVal::equals(Val *o) {
    NumVal *c = dynamic_cast<NumVal*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val == c->val);
    }
}

int NumVal::interp() {
    return this->val;
}

Expr* NumVal::to_expr() {
    return new NumExpr(this);
}

Val* NumVal::add_to(Val *rhs) {
    NumVal *rhsNumVal = dynamic_cast<NumVal*>(rhs);
    
    this->val = this->val + rhsNumVal->val;
    
    return this;
}

Val* NumVal::mult_to(Val *rhs) {
    NumVal *rhsNumVal = dynamic_cast<NumVal*>(rhs);
    
    this->val = this->val * rhsNumVal->val;
    
    return this;
}

TEST_CASE( "equals" ) {
    CHECK( (new NumVal(5))->equals(new NumVal(5)));
    CHECK( (new NumVal(5))->equals(new NumVal(7)) == false);
    CHECK( (new NumVal(5))->equals(NULL) == false);
}
