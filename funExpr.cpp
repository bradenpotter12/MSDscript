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

FunExpr::FunExpr(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(Expr *o) {
    FunExpr *c = dynamic_cast<FunExpr*>(o);
    if (c == NULL) {
        return false;
    }
    return (formal_arg == c->formal_arg) && body->equals(c->body);
}

TEST_CASE( "FunExpr equals" ) {
    CHECK( (new FunExpr("x", new VarExpr("y")))->equals(new BoolExpr(true)) == false);
    
    std::stringstream out("");
    (new FunExpr("x", new VarExpr("y")))->pretty_print(out);
    CHECK( out.str() == "_fun (x) y");
}

Val* FunExpr::interp() {
    return new FunVal(formal_arg, body);
}

Expr* FunExpr::subst(std::string string, Expr *replacement) {
    if (string != formal_arg) {
        return new FunExpr(formal_arg, body->subst(string, replacement));
    }
    return this;
}

TEST_CASE( "FunExpr subst" ) {
    CHECK( (new FunExpr("x", new VarExpr("y")))->subst("y", new VarExpr("z"))->equals(new FunExpr("x", new VarExpr("z"))));
    
    CHECK( (new FunExpr("x", new VarExpr("y")))->subst("x", new VarExpr("z"))->equals(new FunExpr("x", new VarExpr("y"))));
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
