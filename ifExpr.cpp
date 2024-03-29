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
#include "step.hpp"
#include "env.hpp"
#include "cont.hpp"


IfExpr::IfExpr(PTR(Expr) ifExpr, PTR(Expr) thenExpr, PTR(Expr) elseExpr) {
    this->ifExpr = ifExpr;
    this->thenExpr = thenExpr;
    this->elseExpr = elseExpr;
}

bool IfExpr::equals(PTR(Expr) other_expr) {
    PTR(IfExpr) c = CAST(IfExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    return ifExpr->equals(c->ifExpr) && thenExpr->equals(c->thenExpr) && elseExpr->equals(c->elseExpr);
}

PTR(Val)  IfExpr::interp(PTR(Env) env) {
    
    if (ifExpr->interp(env)->equals(NEW(BoolVal)(true))) {
        return thenExpr->interp(env);
    }
    else if (ifExpr->interp(env)->equals(NEW(BoolVal)(false)))
        return elseExpr->interp(env);
    
    throw std::runtime_error("ifExpr must result in boolean value");
}

TEST_CASE( "IfExpr equals & interp" ) {
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(NEW(NumVal)(2)), NEW(BoolExpr)(false)))->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(NEW(NumVal)(2)), NEW(BoolExpr)(false))));
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true), NEW(BoolExpr)(true)))->equals(NEW(BoolExpr)(true)) == false);
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(false)));
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false), NEW(BoolExpr)(false), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(2)));
    
    PTR(EqExpr) eqTrue = NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1)));
    
    CHECK( (NEW(IfExpr)(eqTrue, NEW(BoolExpr)(false), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(false)));
    
    PTR(EqExpr) eqFalse = NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2)));
    
    CHECK( (NEW(IfExpr)(eqFalse, NEW(BoolExpr)(false), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(2)));
    
    PTR(AddExpr) add_numbers = NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2)));
    
    CHECK_THROWS_WITH( (NEW(IfExpr)(add_numbers, NEW(BoolExpr)(false), NEW(BoolExpr)(true)))->interp(NEW(EmptyEnv)()), "ifExpr must result in boolean value");
}

void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = ifExpr;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(thenExpr, elseExpr, Step::env, Step::cont);
}

PTR(Expr) IfExpr::subst(std::string string, PTR(Expr) replacement) {
    return NEW(IfExpr)(ifExpr->subst(string, replacement), thenExpr->subst(string, replacement), elseExpr->subst(string, replacement));
}

TEST_CASE( "IfExpr subst" ) {
    
    PTR(AddExpr) add_with_var = NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)));
    
    PTR(AddExpr) add_with_var_z = NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(2)));
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), add_with_var, NEW(BoolExpr)(true)))->subst("x", NEW(VarExpr)("z"))->equals(NEW(IfExpr)(NEW(BoolExpr)(true), add_with_var_z, NEW(BoolExpr)(true))));
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
    
    CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))->to_string() == "(_if _true _then _false _else _true)");
    
    std::stringstream out("");
    (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))->pretty_print(out);
    CHECK( out.str() == "(_if _true _then _false _else _true)");
}

void IfExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}




