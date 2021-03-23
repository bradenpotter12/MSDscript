//
//  funExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/15/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "val.hpp"
#include "catch.hpp"

FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) other_expr) {
    PTR(FunExpr) c = CAST(FunExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    return (formal_arg == c->formal_arg) && body->equals(c->body);
}

TEST_CASE( "FunExpr equals" ) {
    CHECK( (NEW(FunExpr)("x", NEW(VarExpr)("y")))->equals(NEW(BoolExpr)(true)) == false);
    
    std::stringstream out("");
    (NEW(FunExpr)("x", NEW(VarExpr)("y")))->pretty_print(out);
    CHECK( out.str() == "_fun (x) y");
}

PTR(Val)  FunExpr::interp() {
    return NEW(FunVal)(formal_arg, body);
}

PTR(Expr) FunExpr::subst(std::string string, PTR(Expr) replacement) {
    if (string == formal_arg) {
        return THIS;
    }
    return NEW(FunExpr)(formal_arg, body->subst(string, replacement)); // TODO:  subst supposed to substitue formal_arg and body?
}

TEST_CASE( "FunExpr subst" ) {
    CHECK( (NEW(FunExpr)("x", NEW(VarExpr)("y")))->subst("y", NEW(VarExpr)("z"))->equals(NEW(FunExpr)("x", NEW(VarExpr)("z"))));
    
    CHECK( (NEW(FunExpr)("x", NEW(VarExpr)("y")))->subst("x", NEW(VarExpr)("z"))->equals(NEW(FunExpr)("x", NEW(VarExpr)("y"))));
}

void FunExpr::print(std::ostream& out) {
    out << "_fun (";
    out << formal_arg << ") ";
    body->print(out);
}

std::string FunExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void FunExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}
