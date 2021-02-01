//
//  expr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/26/21.
//

#include <stdio.h>
#include "expr.hpp"
#include <stdexcept>
#include "catch.hpp"

// Num Constructor implementation
    Num::Num(int val) {
    this->val = val;
}

// Num Equals implementation
bool Num::equals(Expr *o) {
    Num *c = dynamic_cast<Num*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val == c->val);
    }
}

// Num Interp implementation
int Num::interp() {
    return this->val;
}

// Add Constructor implementation
Add::Add(Num *lhs, Num *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Add Equals implementation
bool Add::equals(Expr *o) {
    Add *c = dynamic_cast<Add*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs));
    }
}

// Add Interp implementation
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

// Mult Constructor implementation
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Mult Equals implementation
bool Mult::equals(Expr *o) {
    Mult *c = dynamic_cast<Mult*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return ((this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs) ));
    }
}

// Mult Interp implementation
int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

// Variable Constructor implementation
Variable::Variable(std::string string) {
    this->string = string;
}

// Variable Equals implementation
bool Variable::equals(Expr *o) {
    Variable *c = dynamic_cast<Variable*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->string == c->string);
    }
}

// Variable Interp implementation
int Variable::interp() {
    
    throw std::runtime_error("no value for variable");
    
    return 0;
}

TEST_CASE( "Interp" ) {
    CHECK( (new Num(4))->interp() == 4);
    CHECK( (new Num(4))->interp() != 3);
    
    CHECK( (new Add(new Num(4), new Num(3)))->interp() == 7);
    CHECK( (new Add(new Num(4), new Num(3)))->interp() != 6);
    
    CHECK( (new Mult(new Num(4), new Num(3)))->interp() == 12);
    
    CHECK_THROWS_WITH( (new Variable("x"))->interp(), "no value for variable" );
    
}
