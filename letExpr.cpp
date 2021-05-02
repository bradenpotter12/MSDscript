//
//  letExpr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/13/21.
//

#include <stdio.h>
#include "expr.hpp"
#include "catch.hpp"
#include "step.hpp"
#include "env.hpp"
#include "cont.hpp"
#include "val.hpp"

LetExpr::LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr) other_expr) {
    PTR(LetExpr) c = CAST(LetExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return this->lhs == c->lhs &&  this->rhs->equals(c->rhs)
        && this->body->equals(c->body);
    }
}

PTR(Val)  LetExpr::interp(PTR(Env) env) {
    
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhs_val, env);
    
    return body->interp(new_env);
}

void LetExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = Step::env;
    Step::cont = NEW(LetBodyCont)(lhs, body, Step::env, Step::cont);
}

PTR(Expr) LetExpr::subst(std::string string, PTR(Expr) r) {
    if (lhs != string) {
        return NEW(LetExpr)(lhs, rhs->subst(string, r), body->subst(string, r));
    }
    return NEW(LetExpr)(lhs, rhs->subst(string, r), body);
}

TEST_CASE( "LetExpr subst" ) {
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(8)));

    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(6)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(7)));

    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(LetExpr)("y", NEW(NumExpr)(NEW(NumVal)(6)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(6)));
    
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(6))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(12)));
}

void LetExpr::print(std::ostream& out) {
    out << "(_let ";
    out << lhs << '=';
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ')';
}

std::string LetExpr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void LetExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}


TEST_CASE( "LetExpr" ) {
    
    // test constructor lhs
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(5)))))->lhs == "x");
    
    // test constructor rhs
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(5)))))->rhs->equals(NEW(NumExpr)(NEW(NumVal)(5))));
    
    // test constructor body
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(5)))))->body->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(5)))));
    
    // test x is not used, expr == expr
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(NumExpr)(NEW(NumVal)(5))))->equals(NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(NumExpr)(NEW(NumVal)(5)))));
    
    // exprs not equal
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(6)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(3)))))->equals(NEW(VarExpr)("x")) == false);
    
    // values in exprs not equal
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(NumExpr)(NEW(NumVal)(5))))->equals(NEW(LetExpr)("y", NEW(NumExpr)(NEW(NumVal)(5)), NEW(NumExpr)(NEW(NumVal)(5)))) == false);
 
    // test subst
    PTR(Expr) let1 = (NEW(LetExpr)("x",
                          NEW(NumExpr)(NEW(NumVal)(1)),
                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)))));
    CHECK( let1->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(3))))
          ->equals(let1) );
    
    PTR(Expr) let2 = (NEW(LetExpr)("x",
                          NEW(VarExpr)("x"),
                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)))));
    CHECK( let2->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(3))))
          ->equals(NEW(LetExpr)("x",
                           NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(3))),
                           NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))))) );
    
    CHECK( (NEW(LetExpr)("x", NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2)))))->subst("y", NEW(NumExpr)(NEW(NumVal)(8)))->equals(NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))))));
    
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(8)), NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(NEW(NumVal)(9)))
          ->equals(NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(8)), NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(9))))));
    
    CHECK( (NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->subst("y", NEW(NumExpr)(NEW(NumVal)(8)))->equals(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(8)), NEW(NumExpr)(NEW(NumVal)(8)))));
    
    CHECK( (NEW(LetExpr)("x", NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(NEW(NumVal)(8)))->equals(NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(8))))));
    
    // test doesn't replace variable on lhs in Let
    CHECK( (NEW(LetExpr)("z", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32)))))->subst("z", NEW(NumExpr)(NEW(NumVal)(0)))->equals(NEW(LetExpr)("z", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32))))));
    
    CHECK( (NEW(LetExpr)("z", NEW(VarExpr)("z"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32)))))->subst("z", NEW(NumExpr)(NEW(NumVal)(0)))->equals(NEW(LetExpr)("z", NEW(NumExpr)(NEW(NumVal)(0)), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32))))));
    
    CHECK( (NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32)))))->subst("z", NEW(NumExpr)(NEW(NumVal)(0)))->equals(NEW(LetExpr)("z", NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(0)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(NEW(NumVal)(32))))));
    
    // test interp
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(32)))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(34)));
}
