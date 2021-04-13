//
//  callExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/15/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "val.hpp"
#include "catch.hpp"

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) other_expr) {
    PTR(CallExpr) c = CAST(CallExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    return to_be_called->equals(c->to_be_called) && actual_arg->equals(c->actual_arg);
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

TEST_CASE( "CallExpr interp" ) {
    
    CHECK( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("x")), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(2)));
    
    CHECK( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(4)));
    
    CHECK_THROWS_WITH( (NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("y")), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(2)), "no value for variable");
}

PTR(Expr) CallExpr::subst(std::string string, PTR(Expr) replacement) {
    return NEW(CallExpr)(to_be_called->subst(string, replacement), actual_arg->subst(string, replacement));
}

TEST_CASE( "CallExpr Subst" ) {
    CHECK( (NEW(CallExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->subst("x", NEW(VarExpr)("z"))->equals(NEW(CallExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("y"))));
    
    CHECK( (NEW(CallExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->subst("y", NEW(NumExpr)(NEW(NumVal)(3)))->equals(NEW(CallExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(3)))));
}

void CallExpr::print(std::ostream& out) {
    out << '(';
    to_be_called->print(out);
    out << ')';
    out << '(';
    actual_arg->print(out);
    out << ')';
}
std::string CallExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void CallExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    out << '(';
    to_be_called->pretty_print(out);
    out << ')';
    out << '(';
    actual_arg->pretty_print(out);
    out << ')';
}

TEST_CASE( "CallExpr" ) {
    
    CHECK( (NEW(CallExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(10))))->equals(NEW(BoolExpr)(true)) == false);
    
    CHECK( (NEW(CallExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(VarExpr)("x")), NEW(NumExpr)(NEW(NumVal)(2))))->to_string() == "((1+x))(2)");
    
    std::stringstream out("");
    (NEW(CallExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(VarExpr)("x")), NEW(NumExpr)(NEW(NumVal)(2))))->pretty_print(out);
    CHECK( out.str() == "(1 + x)(2)");
}
