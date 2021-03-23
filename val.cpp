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

std::string NumVal::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void NumVal::print(std::ostream &out) {
    out << this->val;
}

Val* NumVal::call(Val *actual_arg) {
    throw std::runtime_error("call method cannot be used on NumVal");
}

TEST_CASE( "equals" ) {
    CHECK( (new NumVal(5))->equals(new NumVal(5)));
    CHECK( (new NumVal(5))->equals(new NumVal(7)) == false);
    CHECK( (new NumVal(5))->equals(new BoolVal(false)) == false);
    
    CHECK_THROWS_WITH((new NumVal(4))->call(new NumVal(10)), "call method cannot be used on NumVal");
}

BoolVal::BoolVal(bool value) {
    this->value = value;
}

bool BoolVal::equals(Val *o) {
    BoolVal *c = dynamic_cast<BoolVal*>(o);
    if (c == NULL) {
        return false;
    }
    return this->value == c->value;
}

int BoolVal::interp() {
    return this->value;
}

Expr* BoolVal::to_expr() {
    return new BoolExpr(this->interp());
}

Val* BoolVal::add_to(Val *rhs) {
    throw std::runtime_error("BoolVal cannot be added");
}

Val* BoolVal::mult_to(Val *rhs) {
    throw std::runtime_error("BoolVal cannot be multiplied");
}

std::string BoolVal::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void BoolVal::print(std::ostream &out) {
    out << "_false";
}

Val* BoolVal::call(Val *actual_arg) {
    throw std::runtime_error("call method cannot be used on BoolVal");
}

TEST_CASE( "BoolVal" ) {
    CHECK( (new BoolVal(false))->interp() == false);
    CHECK( (new BoolVal(false))->equals(new BoolVal(false)));
    CHECK( (new BoolVal(false))->equals(new NumVal(4)) == false);
    
    CHECK_THROWS_WITH((new BoolVal(false))->add_to(new BoolVal(true)), "BoolVal cannot be added");
    CHECK_THROWS_WITH((new BoolVal(false))->add_to(new NumVal(4)), "BoolVal cannot be added");
    CHECK_THROWS_WITH( (new BoolVal(true))->mult_to(new BoolVal(true)), "BoolVal cannot be multiplied");
    CHECK_THROWS_WITH( (new BoolVal(true))->mult_to(new NumVal(4)), "BoolVal cannot be multiplied");
    
    CHECK( (new BoolVal(true))->to_expr()->equals(new BoolExpr(true)));
    CHECK( (new NumVal(1))->to_expr()->equals(new NumExpr(new NumVal(1))));
    
    CHECK( (new BoolVal(false))->to_string() == "_false");
    CHECK_THROWS_WITH((new BoolVal(true))->call(new NumVal(8)), "call method cannot be used on BoolVal");
}
