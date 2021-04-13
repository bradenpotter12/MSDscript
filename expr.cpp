//
//  expr.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/26/21.
//

#include <stdio.h>
#include "expr.hpp"
#include <stdexcept>
#include "catch.hpp"
#include <sstream>
#include <iostream>
#include "val.hpp"
#include "parse.hpp"

#define Var Variable

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


void Expr::pretty_print(std::ostream& out) {
    this->pretty_print_at(print_group_none, out);
}

// Num Constructor implementation
NumExpr::NumExpr(PTR(Val) val) {
    this->val = val;
}

// Num Equals implementation
bool NumExpr::equals(PTR(Expr) other_expr) {
    PTR(NumExpr) c = CAST(NumExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val->equals(c->val));
    }
}

// Num Interp implementation
PTR(Val) NumExpr::interp(PTR(Env) env) {
    return this->val;
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

// Add Constructor implementation
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Add Equals implementation
bool AddExpr::equals(PTR(Expr) other_expr) {
    PTR(AddExpr) c = CAST(AddExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs));
    }
}

// Add Interp implementation
PTR(Val) AddExpr::interp(PTR(Env) env) {
    
    return lhs->interp(env)->add_to(rhs->interp(env));
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

// Mult Constructor implementation
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Mult Equals implementation
bool MultExpr::equals(PTR(Expr) other_expr) {
    PTR(MultExpr) c = CAST(MultExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return ((this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs) ));
    }
}

// Mult Interp implementation
PTR(Val)  MultExpr::interp(PTR(Env) env) {
    PTR(Val)  lhs = this->lhs->interp(env);
    PTR(Val)  rhs = this->rhs->interp(env);
    return lhs->mult_to(rhs);
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

// Variable Constructor implementation
VarExpr::VarExpr(std::string string) {
    this->string = string;
}

// Variable Equals implementation
bool VarExpr::equals(PTR(Expr) other_expr) {
    PTR(VarExpr) c = CAST(VarExpr)(other_expr);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->string == c->string);
    }
}

// Variable Interp implementation
PTR(Val)  VarExpr::interp(PTR(Env) env) {
    
    return env->lookup(this->string);
    
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

/* Let Class *************************************************************/

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

/* TESTS *****************************************************************/

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

    CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(NEW(EmptyEnv)()), "no value for variable" );
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
