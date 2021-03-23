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

FunVal::FunVal(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(Val *o) {
    FunVal *c = dynamic_cast<FunVal*>(o);
    if (c == NULL) {
        return false;
    }
    return (formal_arg == c->formal_arg) && body->equals(c->body);
}

TEST_CASE( "FunVal equals" ) {
    
    CHECK( (new FunVal("x", new VarExpr("y")))->equals(new FunVal("x", new VarExpr("y"))));
    
    CHECK( (new FunVal("x", new VarExpr("y")))->equals(new BoolVal(true)) == false);
}

// I don't think this is needed
int FunVal::interp() {
    NumVal *c = dynamic_cast<NumVal*>(body->interp());
    if (c == NULL) {
        throw std::runtime_error("Expr should interp to NumVal");
    }
    return c->interp();
}

Expr* FunVal::to_expr() {
    return new FunExpr(formal_arg, body);
}

Val* FunVal::add_to(Val *rhs) {
    throw std::runtime_error("FunVal's cannot be added");
}

Val* FunVal::mult_to(Val *rhs) {
    throw std::runtime_error("FunVal's cannot be multiplied");
}

TEST_CASE( "FunVal add_to & mult_to") {
    
    CHECK_THROWS_WITH((new FunVal("x", new VarExpr("y")))->add_to(new NumVal(2)), "FunVal's cannot be added");
    
    CHECK_THROWS_WITH((new FunVal("x", new VarExpr("y")))->mult_to(new NumVal(2)), "FunVal's cannot be multiplied");
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

Val* FunVal::call(Val *actual_arg) {
    body = body->subst(formal_arg, actual_arg->to_expr());
    
    return body->interp();
}

TEST_CASE( "CallExpr interp" ) {
    
    CHECK( (new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(new NumVal(2))))->interp()->equals(new NumVal(2)));
    
    CHECK( (new CallExpr(new FunExpr("x", new MultExpr(new VarExpr("x"), new VarExpr("x"))), new NumExpr(new NumVal(2))))->interp()->equals(new NumVal(4)));
    
    CHECK_THROWS_WITH( (new CallExpr(new FunExpr("x", new VarExpr("y")), new NumExpr(new NumVal(2))))->interp()->equals(new NumVal(2)), "no value for variable");
}

