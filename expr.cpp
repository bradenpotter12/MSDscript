//
//  expr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/26/21.
//

//#include <stdio.h>
#include "expr.hpp"
#include <stdexcept>
#include "catch.hpp"
#include <sstream>
#include <iostream>
#include "val.hpp"
#include "parse.hpp"
#include "step.hpp"
#include "env.hpp"
#include "cont.hpp"

#define Var Variable

void Expr::pretty_print(std::ostream& out) {
    this->pretty_print_at(print_group_none, out);
}

/* NumExpr */
NumExpr::NumExpr(PTR(Val) val) {
    this->val = val;
}

bool NumExpr::equals(PTR(Expr) other_expr) {
    PTR(NumExpr) c = CAST(NumExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val->equals(c->val));
    }
}

PTR(Val) NumExpr::interp(PTR(Env) env) {
    return this->val;
}

void NumExpr::step_interp() {  // return == continue_mode with value and current continuation
    Step::mode = Step::continue_mode;
    Step::val = this->val; // not sure how to make this work, my NumVal constructor takes int
    Step::cont = Step::cont; /* no-op */
}

TEST_CASE( "NumExpr::step_interp" ) {
    PTR(Expr) lhs = NEW(NumExpr)(NEW(NumVal)(1000000));
    PTR(Expr) rhs = NEW(NumExpr)(NEW(NumVal)(1000000));
    PTR(AddExpr) two_mill = NEW(AddExpr)(lhs, rhs);
    
    CHECK(Step::interp_by_steps(two_mill)->to_string() == "2000000");
}

PTR(Expr) NumExpr::subst(std::string string, PTR(Expr) replacement) {
    return THIS;
}

void NumExpr::print(std::ostream &out) {
    PTR(NumVal) c = CAST(NumVal)(this->val);
    out << c->interp();
}

std::string NumExpr::to_string() {
    
    std::stringstream out("");
    this->pretty_print(out);
    
    return out.str();
}

void NumExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

/* AddExpr */
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(PTR(Expr) other_expr) {
    PTR(AddExpr) c = CAST(AddExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs));
    }
}

PTR(Val) AddExpr::interp(PTR(Env) env) {
    PTR(Val) lhs_val = lhs->interp(env);
    PTR(Val) rhs_val = rhs->interp(env);
    return lhs_val->add_to(rhs_val);
}

void AddExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env; /* no-op, could omit */
    Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}

PTR(Expr) AddExpr::subst(std::string string, PTR(Expr) replacement) {
    
    return NEW(AddExpr)(this->lhs->subst(string, replacement), this->rhs->subst(string, replacement));

}

void AddExpr::print(std::ostream &out) {
    out << '(';
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ')';
}

std::string AddExpr::to_string() {
    
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
    
}

void AddExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    
    if (mode == print_group_add || mode == print_group_add_or_mult) {
        out << '(';
    }
    
    lhs->pretty_print_at(print_group_add, out);
    out << " + ";
    rhs->pretty_print_at(print_group_none, out);
    
    if (mode == print_group_add || mode == print_group_add_or_mult) {
        out << ')';
    }
}

/* MultExpr */

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(PTR(Expr) other_expr) {
    PTR(MultExpr) c = CAST(MultExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return ((this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs) ));
    }
}

PTR(Val)  MultExpr::interp(PTR(Env) env) {
    PTR(Val)  lhs = this->lhs->interp(env);
    PTR(Val)  rhs = this->rhs->interp(env);
    return lhs->mult_to(rhs);
}

void MultExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenMultCont)(rhs, Step::env, Step::cont);
}

PTR(Expr) MultExpr::subst(std::string string, PTR(Expr) replacement) {
    
    return NEW(MultExpr)(this->lhs->subst(string, replacement), this->rhs->subst(string, replacement));
}

void MultExpr::print(std::ostream &out) {
    out << '(';
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ')';
}

std::string MultExpr::to_string() {
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
}

void MultExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    if (mode == print_group_add_or_mult) {
        out << '(';
    }
    
    lhs->pretty_print_at(print_group_add_or_mult, out);
    out << " * ";
    rhs->pretty_print_at(print_group_none, out);
    
    if (mode == print_group_add_or_mult) {
        out << ')';
    }
}

/* VarExpr */
VarExpr::VarExpr(std::string string) {
    this->string = string;
}

bool VarExpr::equals(PTR(Expr) other_expr) {
    PTR(VarExpr) c = CAST(VarExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->string == c->string);
    }
}

PTR(Val)  VarExpr::interp(PTR(Env) env) {
    
    return env->lookup(this->string);
}

void VarExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = Step::env->lookup(string);
    Step::cont = Step::cont;
}

TEST_CASE( "VarExpr interp" ) {
    CHECK( (NEW(VarExpr)("x"))->interp(NEW(ExtendedEnv)("x", NEW(NumVal)(3), NEW(EmptyEnv)()))->to_string() == "3");
}

PTR(Expr) VarExpr::subst(std::string string, PTR(Expr) replacement) {

    if (this->string == string) {
        return replacement;
    }
    
    return THIS;
}

void VarExpr::print(std::ostream &out) {
    out << this->string;
}

std::string VarExpr::to_string() {
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
}

void VarExpr::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

// test constructor and equals implementations
TEST_CASE("NumExpr") {
    
    CHECK( (NEW(NumExpr)(NEW(NumVal)(7)))->equals(NEW(VarExpr)("x")) == false);
    
    PTR(NumExpr) c = NEW(NumExpr)(NEW(NumVal)(3));
    PTR(NumExpr) b = NEW(NumExpr)(NEW(NumVal)(3));
    PTR(NumExpr) a = NEW(NumExpr)(NEW(NumVal)(5));
    CHECK(c->val->equals(NEW(NumVal)(3)));
    CHECK(a->val->equals(NEW(NumVal)(5)));
    CHECK(c->equals(b));
    CHECK(c->equals(a) == false);
}

// test constructor and equals implementations
TEST_CASE("AddExpr") {
    PTR(NumExpr) a = NEW(NumExpr)(NEW(NumVal)(3));
    PTR(NumExpr) b = NEW(NumExpr)(NEW(NumVal)(5));
    PTR(AddExpr) expr1 = NEW(AddExpr)(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);
    CHECK(expr1->equals(a) == false);

    PTR(AddExpr) expr2 = NEW(AddExpr)(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    PTR(AddExpr) expr3 = NEW(AddExpr)(a, b);
    CHECK(expr1->equals(expr3));
    
    PTR(Expr) let = NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(1)), NEW(VarExpr)("x"));
    
    CHECK( (let->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(1))));
    CHECK( (NEW(AddExpr)(let, NEW(NumExpr)(NEW(NumVal)(1))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(2)));
    
}

// test constructor and equals implementations
TEST_CASE("MultExpr") {
    PTR(NumExpr) a = NEW(NumExpr)(NEW(NumVal)(3));
    PTR(NumExpr) b = NEW(NumExpr)(NEW(NumVal)(5));
    PTR(MultExpr) expr1 = NEW(MultExpr)(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);

    PTR(MultExpr) expr2 = NEW(MultExpr)(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    PTR(MultExpr) expr3 = NEW(MultExpr)(a, b);
    CHECK(expr1->equals(expr3));
    
    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))))
          ->equals(NEW(VarExpr)("x")) == false);
}

TEST_CASE("VarExpr") {
    PTR(VarExpr) a = NEW(VarExpr)("cat");
    PTR(VarExpr) b = NEW(VarExpr)("b");
    CHECK(a->string == "cat");
    CHECK(b->string == "b");

    PTR(VarExpr) c = NEW(VarExpr)("cat");
    CHECK(a->equals(c));
    CHECK(a->equals(b) == false);

    PTR(NumExpr) one = NEW(NumExpr)(NEW(NumVal)(10));
    CHECK(a->equals(one) == false);
    
    // Wanted to double check variable->equals was working
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")));
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false);
}

TEST_CASE( "Interp" ) {
    CHECK( (NEW(NumExpr)(NEW(NumVal)(4)))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(4)));
    CHECK( (NEW(NumExpr)(NEW(NumVal)(4)))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(3)) == false);

    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(3))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(7)));
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(3))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(6)) == false);
    
    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(3))))->interp(NEW(EmptyEnv)())->equals(NEW(NumVal)(12)));
}

TEST_CASE( "subst" ) {
    
    CHECK( (NEW(NumExpr)(NEW(NumVal)(8)))->subst("x", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(NEW(NumVal)(8))));
    
    CHECK( (NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(VarExpr)("y")));
    
    CHECK( (NEW(VarExpr)("x"))->subst("z", NEW(VarExpr)("y"))
          ->equals(NEW(VarExpr)("x")));
    
    CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(7))))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(7)))) );
    
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(7)), NEW(VarExpr)("x")))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(7)), NEW(VarExpr)("y"))) );

    CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))) );

    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(7))))
          ->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(7)))));

    CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(7))))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(7)))) );

    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(7)), NEW(VarExpr)("x")))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(7)), NEW(VarExpr)("y"))) );

    CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))
           ->subst("x", NEW(VarExpr)("y"))
           ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))) );

    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(7))))
          ->subst("x", NEW(VarExpr)("y"))->equals(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(7)))));

    CHECK( (NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("7"))
          ->equals(NEW(VarExpr)("7")));

    CHECK( (NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(VarExpr)("y")));

    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))), NEW(NumExpr)(NEW(NumVal)(2))))
          ->subst("x", NEW(NumExpr)(NEW(NumVal)(7)))->equals( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(7)), NEW(NumExpr)(NEW(NumVal)(1))), NEW(NumExpr)(NEW(NumVal)(2))))));
}

TEST_CASE( "print" ) {
    
    {
    std::stringstream out("");
    (NEW(NumExpr)(NEW(NumVal)(7)))->print(out);
    CHECK( out.str() == "7");
    }
    
    {
    std::stringstream out("");
    (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))))->print(out);
    CHECK( out.str() == "(1+2)");
    }
    
    {
    std::stringstream out("");
    (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(1))))->print(out);
    CHECK( out.str() == "((1+2)+1)");
    }
    
    {
    std::stringstream out("");
    (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(1))))->print(out);
    CHECK( out.str() == "((1+2)*1)");
    }
    
    {
    std::stringstream out("");
    (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(1))))->print(out);
    CHECK( out.str() == "((1*2)*1)");
    }
    
    {
    std::stringstream out("");
    (NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(1))))->print(out);
    CHECK( out.str() == "((x+2)*1)");
    }
    
    {
    std::stringstream out("");
    (NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(2))), NEW(VarExpr)("y")))->print(out);
    CHECK( out.str() == "((x+2)*y)");
    }
}

TEST_CASE( "to_string" ) {
    CHECK( (NEW(NumExpr)(NEW(NumVal)(7)))->to_string() == "7");
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))))->to_string() == "1 + 2");
    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(4))))->to_string() == "2 * 4");
    CHECK( (NEW(VarExpr)("x"))->to_string() == "x");
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(NEW(NumVal)(3)), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(2)))), NEW(VarExpr)("x"))))->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE( "pretty_print_at" ) {
    CHECK( (NEW(NumExpr)(NEW(NumVal)(7)))->to_string() == "7");
    
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3)))))
          ->to_string() == "1 + 2 + 3");
    
    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(3))))
          ->to_string() == "(1 + 2) + 3");

    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(3)), NEW(NumExpr)(NEW(NumVal)(4)))))->to_string() == "(1 + 2) + 3 + 4");
    
    CHECK( (NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(3)), NEW(NumExpr)(NEW(NumVal)(4)))))
          ->to_string() == "2 * 3 * 4");
    
    CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))), NEW(NumExpr)(NEW(NumVal)(4))))
          ->to_string() == "(2 * 3) * 4");
    
    CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(3)), NEW(NumExpr)(NEW(NumVal)(4)))))->to_string() == "(1 * 2) * 3 * 4");
    
    CHECK( (NEW(VarExpr)("x"))->to_string() == "x");
    
    CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2)))))->to_string() == "x + 1 + 2");
    
    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(NEW(NumVal)(1))), NEW(NumExpr)(NEW(NumVal)(2))))->to_string() == "(x + 1) + 2");
    
    CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))), NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(4)), NEW(NumExpr)(NEW(NumVal)(5)))))->to_string() == "2 * 3 + 4 + 5");
    
    CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(NumExpr)(NEW(NumVal)(1))))
          ->to_string() == "2 * 2 + 1");
        
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(2)))))
          ->to_string() == "1 + 2 * 2");
    
    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2))), NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3)))))->to_string() == "(1 + 2) + 2 * 3");
    
    CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(NEW(NumVal)(2)), NEW(NumExpr)(NEW(NumVal)(3))), NEW(MultExpr)(NEW(NumExpr)(NEW(NumVal)(1)), NEW(NumExpr)(NEW(NumVal)(2)))))->to_string() == "(2 + 3) + 1 * 2");
}

TEST_CASE( "pretty_print" ) {
    std::stringstream out("");
    (NEW(VarExpr)("x"))->pretty_print(out);
    CHECK( out.str() == "x");
    
    std::stringstream out2("");
    (NEW(LetExpr)("x", NEW(NumExpr)(NEW(NumVal)(5)), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(NEW(NumVal)(3)), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(NEW(NumVal)(2)))), NEW(VarExpr)("x"))))->pretty_print(out2);
    CHECK( out2.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}
