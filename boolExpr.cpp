//
//  boolExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/8/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "val.hpp"
#include "catch.hpp"

BoolExpr::BoolExpr(bool val) {
    
    this->boolVal = new BoolVal(val);
}

bool BoolExpr::equals(Expr *o) {
    BoolExpr *c = dynamic_cast<BoolExpr*>(o);
    if (c == NULL) {
        return false;
    }
    return this->boolVal->equals(c->boolVal);
}

Val* BoolExpr::interp() {
    return this->boolVal;
}

bool BoolExpr::has_variable() {
    return false; // BoolExpr doesn't have a variable
}

Expr* BoolExpr::subst(std::string string, Expr *replacement) {
    return this; // No variables to substitue
}

void BoolExpr::print(std::ostream& out) {
    if (this->boolVal->equals(new BoolVal(true))) {
        out << "_true";
    }
    else
        out << "_false";
}

std::string BoolExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void BoolExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

TEST_CASE( "BoolExpr" ) {
    
    CHECK( ((new BoolExpr(true))->equals(new BoolExpr(true))));
    CHECK( ((new BoolExpr(true))->equals(new BoolExpr(false))) == false);
    CHECK( ((new BoolExpr(true))->equals(new NumExpr(new NumVal(1)))) == false);
    
    CHECK( (new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    CHECK( (new BoolExpr(false))->interp()->equals(new BoolVal(false)));
    
    CHECK( (new BoolExpr(true))->has_variable() == false);
    
    CHECK( (new BoolExpr(true))->subst("y", new VarExpr("x"))->equals(new BoolExpr(true)));
    
    {
        std::stringstream out("");
        (new BoolExpr(true))->print(out);
        CHECK( (out.str() == "_true"));
    }
    
    {
        std::stringstream out("");
        (new BoolExpr(false))->print(out);
        CHECK( (out.str() == "_false"));
    }
    
    CHECK( (new BoolExpr(true))->to_string() == "_true");
    CHECK( (new BoolExpr(false))->to_string() == "_false");
}
