//
//  funVal.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/15/21.
//

#include <stdio.h>
#include "val.hpp"
#include "expr.hpp"
#include "catch.hpp"

FunVal::FunVal(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(PTR(Val) other_val) {
    PTR(FunVal) c = CAST(FunVal)(other_val);
    if (c == NULL) {
        return false;
    }
    return (formal_arg == c->formal_arg) && body->equals(c->body);
}

TEST_CASE( "FunVal equals" ) {
    
    CHECK( (NEW(FunVal)("x", NEW(VarExpr)("y")))->equals(NEW(FunVal)("x", NEW(VarExpr)("y"))));
    
    CHECK( (NEW(FunVal)("x", NEW(VarExpr)("y")))->equals(NEW(BoolVal)(true)) == false);
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(formal_arg, body);
}

PTR(Val) FunVal::add_to(PTR(Val) rhs) {
    throw std::runtime_error("FunVal's cannot be added");
}

PTR(Val) FunVal::mult_to(PTR(Val) rhs) {
    throw std::runtime_error("FunVal's cannot be multiplied");
}

TEST_CASE( "FunVal add_to & mult_to") {
    
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(VarExpr)("y")))->add_to(NEW(NumVal)(2)), "FunVal's cannot be added");
    
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(VarExpr)("y")))->mult_to(NEW(NumVal)(2)), "FunVal's cannot be multiplied");
}

std::string FunVal::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void FunVal::print(std::ostream &out) {
    out << "_fun (";
    out << formal_arg << ") ";
    body->print(out);
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    body = body->subst(formal_arg, actual_arg->to_expr());
    
    return body->interp();
}

TEST_CASE( "FunVal call" ) {
    CHECK( (NEW(FunVal)("x", NEW(VarExpr)("x")))->call(NEW(NumVal)(10))->to_string() == "10");
    
    CHECK( (NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)))))->call(NEW(NumVal)(10))->to_string() == "12");
    
    CHECK( (NEW(FunVal)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)))))->call(NEW(NumVal)(10))->to_string() == "20");
    
    CHECK( (NEW(FunVal)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))->call(NEW(NumVal)(10))->to_string() == "100");
}

TEST_CASE( "CallExpr interp" ) {
    
    CHECK( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("x")), NEW(NumExpr)(NEW(NumVal)(2))))->interp()->equals(NEW(NumVal)(2)));
    
    CHECK( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))), NEW(NumExpr)(NEW(NumVal)(2))))->interp()->equals(NEW(NumVal)(4)));
    
    CHECK_THROWS_WITH( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("y")), NEW(NumExpr)(NEW(NumVal)(2))))->interp()->equals(NEW(NumVal)(2)), "no value for variable");
}

