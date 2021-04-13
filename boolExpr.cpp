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
    
    this->boolVal = NEW(BoolVal)(val);
}

bool BoolExpr::equals(PTR(Expr) other_expr) {
    PTR(BoolExpr) c = CAST(BoolExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    return this->boolVal->equals(c->boolVal);
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return this->boolVal;
}

PTR(Expr) BoolExpr::subst(std::string string, PTR(Expr) replacement) {
    return THIS; // No variables to substitue
}

void BoolExpr::print(std::ostream& out) {
    if (this->boolVal->equals(NEW(BoolVal)(true))) {
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
    
    CHECK( ((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true))));
    CHECK( ((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false))) == false);
    CHECK( ((NEW(BoolExpr)(true))->equals(NEW(NumExpr)(NEW(NumVal)(1)))) == false);
    
    CHECK( (NEW(BoolExpr)(true))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));
    CHECK( (NEW(BoolExpr)(false))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(false)));
        
    CHECK( (NEW(BoolExpr)(true))->subst("y", NEW(VarExpr)("x"))->equals(NEW(BoolExpr)(true)));
    
    {
        std::stringstream out("");
        (NEW(BoolExpr)(true))->print(out);
        CHECK( (out.str() == "_true"));
    }
    
    {
        std::stringstream out("");
        (NEW(BoolExpr)(false))->print(out);
        CHECK( (out.str() == "_false"));
    }
    
    CHECK( (NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK( (NEW(BoolExpr)(false))->to_string() == "_false");
    
    {
        std::stringstream out("");
        (NEW(BoolExpr)(false))->pretty_print(out);
        CHECK( (out.str() == "_false"));
    }
}
