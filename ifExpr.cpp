//
//  ifExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/8/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "catch.hpp"
#include "val.hpp"


IfExpr::IfExpr(Expr *ifExpr, Expr *thenExpr, Expr *elseExpr) {
    this->ifExpr = ifExpr;
    this->thenExpr = thenExpr;
    this->elseExpr = elseExpr;
}

bool IfExpr::equals(Expr *o) {
    IfExpr *c = dynamic_cast<IfExpr*>(o);
    if (c == NULL) {
        return false;
    }
    return ifExpr->equals(c->ifExpr) && thenExpr->equals(c->thenExpr) && elseExpr->equals(c->elseExpr);
}

Val* IfExpr::interp() {
    
    if (ifExpr->interp()->equals(new BoolVal(true))) {
        return thenExpr->interp();
    }
    else if (ifExpr->interp()->equals(new BoolVal(false)))
        return elseExpr->interp();
    
    throw std::runtime_error("ifExpr must result in boolean value");
}

TEST_CASE( "IfExpr equals & interp" ) {
    CHECK( (new IfExpr(new BoolExpr(true), new NumExpr(new NumVal(2)), new BoolExpr(false)))->equals(new IfExpr(new BoolExpr(true), new NumExpr(new NumVal(2)), new BoolExpr(false))));
    
    CHECK( (new IfExpr(new BoolExpr(true), new BoolExpr(true), new BoolExpr(true)))->equals(new BoolExpr(true)) == false);
    
    CHECK( (new IfExpr(new BoolExpr(true), new BoolExpr(false), new BoolExpr(true)))->interp()->equals(new BoolVal(false)));
    
    CHECK( (new IfExpr(new BoolExpr(false), new BoolExpr(false), new BoolExpr(true)))->interp()->equals(new BoolVal(true)));
    
    CHECK( (new IfExpr(new BoolExpr(false), new BoolExpr(false), new NumExpr(new NumVal(2))))->interp()->equals(new NumVal(2)));
    
    EqExpr *eqTrue = new EqExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(1)));
    
    CHECK( (new IfExpr(eqTrue, new BoolExpr(false), new NumExpr(new NumVal(2))))->interp()->equals(new BoolVal(false)));
    
    EqExpr *eqFalse = new EqExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2)));
    
    CHECK( (new IfExpr(eqFalse, new BoolExpr(false), new NumExpr(new NumVal(2))))->interp()->equals(new NumVal(2)));
    
    AddExpr *add_numbers = new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2)));
    
    CHECK_THROWS_WITH( (new IfExpr(add_numbers, new BoolExpr(false), new BoolExpr(true)))->interp(), "ifExpr must result in boolean value");
}

Expr* IfExpr::subst(std::string string, Expr *replacement) {
    return new IfExpr(ifExpr->subst(string, replacement), thenExpr->subst(string, replacement), elseExpr->subst(string, replacement));
}

TEST_CASE( "IfExpr subst" ) {
    
    AddExpr *add_with_var = new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2)));
    
    AddExpr *add_with_var_z = new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(2)));
    
    CHECK( (new IfExpr(new BoolExpr(true), add_with_var, new BoolExpr(true)))->subst("x", new VarExpr("z"))->equals(new IfExpr(new BoolExpr(true), add_with_var_z, new BoolExpr(true))));
}

void IfExpr::print(std::ostream& out) {
    out << "(_if ";
    ifExpr->print(out);
    out << " _then ";
    thenExpr->print(out);
    out << " _else ";
    elseExpr->print(out);
    out << ')';
}

std::string IfExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

TEST_CASE( "IfExpr print & to_string" ) {
    
    CHECK( (new IfExpr(new BoolExpr(true), new BoolExpr(false), new BoolExpr(true)))->to_string() == "(_if _true _then _false _else _true)");
    
    std::stringstream out("");
    (new IfExpr(new BoolExpr(true), new BoolExpr(false), new BoolExpr(true)))->pretty_print(out);
    CHECK( out.str() == "(_if _true _then _false _else _true)");
}

void IfExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}




