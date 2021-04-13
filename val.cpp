//
//  val.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/3/21.
//

#include "val.hpp"
#include "expr.hpp"
#include "catch.hpp"
#include <limits.h>
#include <cmath>


// constructor
NumVal::NumVal(int val) {
    this->val = val;
}

bool NumVal::equals(PTR(Val) other_val) {
    PTR(NumVal) c = CAST(NumVal)(other_val);
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

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(THIS);
}

PTR(Val) NumVal::add_to(PTR(Val) rhs) {
    PTR(NumVal) rhsNumVal = CAST(NumVal)(rhs);
    
    int tempNum = (unsigned)this->val;
    int absValueOfThis = std::abs(tempNum);
    
    if (absValueOfThis > INT_MAX - (unsigned)rhsNumVal->val) {
        throw std::runtime_error("Arithmetic overflow, input numbers result in answer that is too large");
    }
    
    return NEW(NumVal)((unsigned)this->val + (unsigned)rhsNumVal->val);
}

PTR(Val) NumVal::mult_to(PTR(Val) rhs) {
    PTR(NumVal) rhsNumVal = CAST(NumVal)(rhs);
    
    if ((unsigned)this->val > INT_MAX / rhsNumVal->val) {
        throw std::runtime_error("Arithmetic overflow, input numbers result in answer that is too large");
    }
    
    return NEW(NumVal)((unsigned)this->val * (unsigned)rhsNumVal->val);
}

std::string NumVal::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void NumVal::print(std::ostream &out) {
    out << this->val;
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("call method cannot be used on NumVal");
}

TEST_CASE( "equals" ) {
    CHECK( (NEW(NumVal)(5))->equals(NEW(NumVal)(5)));
    CHECK( (NEW(NumVal)(5))->equals(NEW(NumVal)(7)) == false);
    CHECK( (NEW(NumVal)(5))->equals(NEW(BoolVal)(false)) == false);
    
    CHECK_THROWS_WITH((NEW(NumVal)(4))->call(NEW(NumVal)(10)), "call method cannot be used on NumVal");
}

BoolVal::BoolVal(bool value) {
    this->value = value;
}

bool BoolVal::equals(PTR(Val) other_val) {
    PTR(BoolVal) c = CAST(BoolVal)(other_val);
    if (c == NULL) {
        return false;
    }
    return this->value == c->value;
}

int BoolVal::interp() {
    return this->value;
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(this->interp());
}

PTR(Val) BoolVal::add_to(PTR(Val) rhs) {
    throw std::runtime_error("BoolVal cannot be added");
}

PTR(Val) BoolVal::mult_to(PTR(Val) rhs) {
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

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("call method cannot be used on BoolVal");
}

TEST_CASE( "BoolVal" ) {
    CHECK( (NEW(BoolVal)(false))->interp() == false);
    CHECK( (NEW(BoolVal)(false))->equals(NEW(BoolVal)(false)));
    CHECK( (NEW(BoolVal)(false))->equals(NEW(NumVal)(4)) == false);
    
    CHECK_THROWS_WITH((NEW(BoolVal)(false))->add_to(NEW(BoolVal)(true)), "BoolVal cannot be added");
    CHECK_THROWS_WITH((NEW(BoolVal)(false))->add_to(NEW(NumVal)(4)), "BoolVal cannot be added");
    CHECK_THROWS_WITH( (NEW(BoolVal)(true))->mult_to(NEW(BoolVal)(true)), "BoolVal cannot be multiplied");
    CHECK_THROWS_WITH( (NEW(BoolVal)(true))->mult_to(NEW(NumVal)(4)), "BoolVal cannot be multiplied");
    
    CHECK( (NEW(BoolVal)(true))->to_expr()->equals(NEW(BoolExpr)(true)));
    CHECK( (NEW(NumVal)(1))->to_expr()->equals(NEW(NumExpr)(NEW(NumVal)(1))));
    
    CHECK( (NEW(BoolVal)(false))->to_string() == "_false");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->call(NEW(NumVal)(8)), "call method cannot be used on BoolVal");
}
