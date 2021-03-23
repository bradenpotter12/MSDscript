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

CallExpr::CallExpr(Expr *to_be_called, Expr *actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(Expr *o) {
    CallExpr *c = dynamic_cast<CallExpr*>(o);
    if (c == NULL) {
        return false;
    }
    return to_be_called->equals(c->to_be_called) && actual_arg->equals(c->actual_arg);
}

Val* CallExpr::interp() {
    return to_be_called->interp()->call(actual_arg->interp());
}

Expr* CallExpr::subst(std::string string, Expr *replacement) {
    return new CallExpr(to_be_called->subst(string, replacement), actual_arg->subst(string, replacement));
}

TEST_CASE( "CallExpr Subst" ) {
    CHECK( (new CallExpr(new VarExpr("x"), new VarExpr("y")))->subst("x", new VarExpr("z"))->equals(new CallExpr(new VarExpr("z"), new VarExpr("y"))));
    
    CHECK( (new CallExpr(new VarExpr("x"), new VarExpr("y")))->subst("y", new NumExpr(new NumVal(3)))->equals(new CallExpr(new VarExpr("x"), new NumExpr(new NumVal(3)))));
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
    
    CHECK( (new CallExpr(new VarExpr("x"), new NumExpr(new NumVal(10))))->equals(new BoolExpr(true)) == false);
    
    CHECK( (new CallExpr(new AddExpr(new NumExpr(new NumVal(1)), new VarExpr("x")), new NumExpr(new NumVal(2))))->to_string() == "((1+x))(2)");
    
    std::stringstream out("");
    (new CallExpr(new AddExpr(new NumExpr(new NumVal(1)), new VarExpr("x")), new NumExpr(new NumVal(2))))->pretty_print(out);
    CHECK( out.str() == "(1 + x)(2)");
}
