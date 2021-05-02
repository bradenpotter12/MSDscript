//
//  eqExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/13/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "catch.hpp"
#include "step.hpp"
#include "val.hpp"
#include "cont.hpp"

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(PTR(Expr) other_expr) {
    PTR(EqExpr) c = CAST(EqExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    return lhs->equals(c->lhs) && rhs->equals(c->rhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env) {
    
    if (lhs->interp(env)->equals(rhs->interp(env)))
        return NEW(BoolVal)(true);
    
    return NEW(BoolVal)(false);
}

void EqExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenCompCont)(rhs, Step::env, Step::cont);
}

PTR(Expr) EqExpr::subst(std::string string, PTR(Expr)  replacement) {
     
    return NEW(EqExpr)(lhs->subst(string, replacement), rhs->subst(string, replacement));
}

TEST_CASE( "EqExpr subst" ) {
    CHECK( (NEW(EqExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))->subst("x", NEW(VarExpr)("z"))->equals(NEW(EqExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("z"))));
}

void EqExpr::print(std::ostream& out) {
    out << '(';
    lhs->print(out);
    out << "==";
    rhs->print(out);
    out << ')';
}

TEST_CASE( "EqExpr print" ) {
    
    {
    std::stringstream out("");
    (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(1))))->print(out);
    CHECK( out.str() == "(2==1)" );
    }
    
    std::stringstream out("");
    (NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))), NEW(NumExpr)(NEW(NumVal)(2))))->print(out);
    CHECK( out.str() == "((2+3)==2)");
    
}

std::string EqExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

TEST_CASE( "EqExpr to_string" ) {
    CHECK( ((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))), NEW(NumExpr)(NEW(NumVal)(2)))))->to_string() == "((2+3)==2)");
}

void EqExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    out << '(';
    lhs->pretty_print(out);
    out << " == ";
    rhs->pretty_print(out);
    out << ')';
}

TEST_CASE( "EqExpr" ) {
    
    // Test pretty_print
    std::stringstream out("");
    (NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true)))->pretty_print(out);
    CHECK( out.str() == "(_true == _true)");
    
    // Test equals()
    CHECK( (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1))))->equals(NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1)))));
    
    CHECK( (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1))))->equals(NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2)))) == false);
    
    CHECK( (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1))))->equals(NEW(VarExpr)("x")) == false);
    
    // Test interp()
    CHECK( (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(false)) );

    CHECK( (NEW(EqExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(1))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)) );

    CHECK( (NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(3))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));
    
    PTR(Expr) let_6 = NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(3)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(3))));
    
    CHECK( (NEW(EqExpr)(let_6, NEW(NumExpr)(NEW(NumVal)(6))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));
    
    PTR(Expr) let_2 = NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))));
    
    CHECK( (NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(4))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));

    CHECK( (NEW(EqExpr)(NEW(AddExpr)(let_2, NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(6))))->interp(NEW(EmptyEnv)())->equals(NEW(BoolVal)(true)));
}
