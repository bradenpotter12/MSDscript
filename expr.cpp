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

void Expr::pretty_print(std::ostream& out) {
    this->pretty_print_at(print_group_none, out);
}

// Num Constructor implementation
    NumExpr::NumExpr(Val *val) {
    this->val = val;
}

// Num Equals implementation
bool NumExpr::equals(Expr *o) {
    NumExpr *c = dynamic_cast<NumExpr*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val->equals(c->val));
    }
}

// Num Interp implementation
Val* NumExpr::interp() {
    return this->val;
}

// Num has_variable implementation
bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string string, Expr *e) {
    return this;
}

void NumExpr::print(std::ostream &out) {
    out << this->val->interp();
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
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Add Equals implementation
bool AddExpr::equals(Expr *o) {
    AddExpr *c = dynamic_cast<AddExpr*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs));
    }
}

// Add Interp implementation
Val* AddExpr::interp() {
    Val * lhs = this->lhs->interp();
    Val * rhs = this->rhs->interp();
    
    return lhs->add_to(rhs);
}

// Add has_variable implementation
bool AddExpr::has_variable() {
    
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* AddExpr::subst(std::string string, Expr *exprSub) {
    
    return new AddExpr(this->lhs->subst(string, exprSub), this->rhs->subst(string, exprSub));

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
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Mult Equals implementation
bool MultExpr::equals(Expr *o) {
    MultExpr *c = dynamic_cast<MultExpr*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return ((this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs) ));
    }
}

// Mult Interp implementation
Val* MultExpr::interp() {
    Val* lhs = this->lhs->interp();
    Val* rhs = this->rhs->interp();
    return lhs->mult_to(rhs);
}

// Mult has_variable implementation
bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* MultExpr::subst(std::string string, Expr *replacement) {
    
    return new MultExpr(this->lhs->subst(string, replacement), this->rhs->subst(string, replacement));
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
bool VarExpr::equals(Expr *o) {
    VarExpr *c = dynamic_cast<VarExpr*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->string == c->string);
    }
}

// Variable Interp implementation
Val* VarExpr::interp() {
    
    throw std::runtime_error("no value for variable");
    
}

// Variable has_variable implementation
bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(std::string string, Expr *replacement) {

    if (this->string == string) {
        return replacement;
    }
    
    return this;
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

LetExpr::LetExpr(std::string lhs, Expr * rhs, Expr * body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(Expr *o) {
    LetExpr *c = dynamic_cast<LetExpr*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return this->lhs == c->lhs &&  this->rhs->equals(c->rhs)
        && this->body->equals(c->body);
    }
}

Val* LetExpr::interp() {
    
    this->rhs = new NumExpr(rhs->interp());
    this->body = body->subst(lhs, rhs);
    
    return body->interp();
}

bool LetExpr::has_variable() {
    return body->has_variable() || rhs->has_variable();
}

Expr* LetExpr::subst(std::string string, Expr *e) {
    if (lhs != string) {
        return new LetExpr(lhs, rhs->subst(string, e), body->subst(string, e));
    }
    return new LetExpr(lhs, rhs->subst(string, e), body);
}

void LetExpr::print(std::ostream& out) {
    out << "(_let ";
    out << lhs << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
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

TEST_CASE( "Let" ) {
    
    // test constructor lhs
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))))->lhs == "x");
    
    // test constructor rhs
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))))->rhs->equals(new NumExpr(new NumVal(5))));
    
    // test constructor body
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))))->body->equals(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))));
    
    // test x is not used, expr == expr
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new NumExpr(new NumVal(5))))->equals(new LetExpr("x", new NumExpr(new NumVal(5)), new NumExpr(new NumVal(5)))));
    
    // exprs not equal
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(6)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(3)))))->equals(new VarExpr("x")) == false);
    
    // values in exprs not equal
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new NumExpr(new NumVal(5))))->equals(new LetExpr("y", new NumExpr(new NumVal(5)), new NumExpr(new NumVal(5)))) == false);
 
    // test subst
    Expr *let1 = (new LetExpr("x",
                          new NumExpr(new NumVal(1)),
                          new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2)))));
    CHECK( let1->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(3))))
          ->equals(let1) );
    
    Expr *let2 = (new LetExpr("x",
                          new VarExpr("x"),
                          new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2)))));
    CHECK( let2->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(3))))
          ->equals(new LetExpr("x",
                           new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(3))),
                           new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))))) );
    
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2)))))->subst("y", new NumExpr(new NumVal(8)))->equals(new LetExpr("x", new NumExpr(new NumVal(8)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))))));
    
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(8)), new AddExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))), new VarExpr("y"))))->subst("y", new NumExpr(new NumVal(9)))
          ->equals(new LetExpr("x", new NumExpr(new NumVal(8)), new AddExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(9))))));
    
    CHECK( (new AddExpr(new VarExpr("y"), new VarExpr("y")))->subst("y", new NumExpr(new NumVal(8)))->equals(new AddExpr(new NumExpr(new NumVal(8)), new NumExpr(new NumVal(8)))));
    
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(new NumVal(8)))->equals(new LetExpr("x", new NumExpr(new NumVal(8)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(8))))));
    
    // test doesn't replace variable on lhs in Let
    CHECK( (new LetExpr("z", new VarExpr("x"), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32)))))->subst("z", new NumExpr(new NumVal(0)))->equals(new LetExpr("z", new VarExpr("x"), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32))))));
    
    CHECK( (new LetExpr("z", new VarExpr("z"), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32)))))->subst("z", new NumExpr(new NumVal(0)))->equals(new LetExpr("z", new NumExpr(new NumVal(0)), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32))))));
    
    CHECK( (new LetExpr("z", new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(2))), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32)))))->subst("z", new NumExpr(new NumVal(0)))->equals(new LetExpr("z", new AddExpr(new NumExpr(new NumVal(0)), new NumExpr(new NumVal(2))), new AddExpr(new VarExpr("z"), new NumExpr(new NumVal(32))))));
    
    // test has_variable
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))))->has_variable());
    
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(5)))))->has_variable());
    
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(2)), new AddExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(5)))))->has_variable() == false);
    
    CHECK( (new LetExpr("x", new VarExpr("z"), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(5)))))->has_variable());
    
    // test interp
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(2)), new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(32)))))->interp()->equals(new NumVal(34)));
    
}

// test constructor and equals implementations
TEST_CASE("NumExpr") {
    
    CHECK( (new NumExpr(new NumVal(7)))->equals(new VarExpr("x")) == false);
    
    NumExpr *c = new NumExpr(new NumVal(3));
    NumExpr *b = new NumExpr(new NumVal(3));
    NumExpr *a = new NumExpr(new NumVal(5));
    CHECK(c->val->equals(new NumVal(3)));
    CHECK(a->val->equals(new NumVal(5)));
    CHECK(c->equals(b));
    CHECK(c->equals(a) == false);
}

// test constructor and equals implementations
TEST_CASE("Add") {
    NumExpr *a = new NumExpr(new NumVal(3));
    NumExpr *b = new NumExpr(new NumVal(5));
    AddExpr *expr1 = new AddExpr(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);
    CHECK(expr1->equals(a) == false);

    AddExpr *expr2 = new AddExpr(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    AddExpr *expr3 = new AddExpr(a, b);
    CHECK(expr1->equals(expr3));
}

// test constructor and equals implementations
TEST_CASE("Mult") {
    NumExpr *a = new NumExpr(new NumVal(3));
    NumExpr *b = new NumExpr(new NumVal(5));
    MultExpr *expr1 = new MultExpr(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);

    MultExpr *expr2 = new MultExpr(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    MultExpr *expr3 = new MultExpr(a, b);
    CHECK(expr1->equals(expr3));
    
    CHECK( (new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3))))
          ->equals(new VarExpr("x")) == false);
}

TEST_CASE("Variable") {
    VarExpr *a = new VarExpr("cat");
    VarExpr *b = new VarExpr("b");
    CHECK(a->string == "cat");
    CHECK(b->string == "b");

    VarExpr *c = new VarExpr("cat");
    CHECK(a->equals(c));
    CHECK(a->equals(b) == false);

    NumExpr *one = new NumExpr(new NumVal(10));
    CHECK(a->equals(one) == false);
    
    // Wanted to double check variable->equals was working
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")));
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false);
}

TEST_CASE( "Interp" ) {
    CHECK( (new NumExpr(new NumVal(4)))->interp()->equals(new NumVal(4)));
    CHECK( (new NumExpr(new NumVal(4)))->interp()->equals(new NumVal(3)) == false);
    
    CHECK( (new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(3))))->interp()->equals(new NumVal(7)));
    CHECK( (new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(3))))->interp()->equals(new NumVal(6)) == false);
    
    CHECK( (new MultExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(3))))->interp()->equals(new NumVal(12)));
    
    CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );
}

TEST_CASE( "has_variable" ) {
    CHECK( (new NumExpr(new NumVal(4)))->has_variable() == false);
    
    CHECK( (new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(3))))->has_variable() == false);
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(4))))->has_variable() == true);
    CHECK( (new AddExpr(new NumExpr(new NumVal(2)), new VarExpr("x")))->has_variable() == true);
    CHECK( (new AddExpr(new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
    
    CHECK( (new MultExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(8))))->has_variable() == false);
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(new NumVal(3))))->has_variable() == true);
    CHECK( (new MultExpr(new NumExpr(new NumVal(2)), new VarExpr("x")))->has_variable() == true);
    CHECK( (new MultExpr(new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
}

TEST_CASE( "subst" ) {
    
    CHECK( (new NumExpr(new NumVal(8)))->subst("x", new VarExpr("y"))->equals(new NumExpr(new NumVal(8))));
    
    CHECK( (new VarExpr("x"))->subst("x", new VarExpr("y"))
          ->equals(new VarExpr("y")));
    
    CHECK( (new VarExpr("x"))->subst("z", new VarExpr("y"))
          ->equals(new VarExpr("x")));
    
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(7))))
           ->subst("x", new VarExpr("y"))
           ->equals(new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(7)))) );
    
    CHECK( (new AddExpr(new NumExpr(new NumVal(7)), new VarExpr("x")))
           ->subst("x", new VarExpr("y"))
           ->equals(new AddExpr(new NumExpr(new NumVal(7)), new VarExpr("y"))) );

    CHECK( (new AddExpr(new VarExpr("x"), new VarExpr("x")))
           ->subst("x", new VarExpr("y"))
           ->equals(new AddExpr(new VarExpr("y"), new VarExpr("y"))) );

    CHECK( (new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(7))))
          ->subst("x", new VarExpr("y"))
          ->equals(new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(7)))));

    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(new NumVal(7))))
           ->subst("x", new VarExpr("y"))
           ->equals(new MultExpr(new VarExpr("y"), new NumExpr(new NumVal(7)))) );

    CHECK( (new MultExpr(new NumExpr(new NumVal(7)), new VarExpr("x")))
           ->subst("x", new VarExpr("y"))
           ->equals(new MultExpr(new NumExpr(new NumVal(7)), new VarExpr("y"))) );

    CHECK( (new MultExpr(new VarExpr("x"), new VarExpr("x")))
           ->subst("x", new VarExpr("y"))
           ->equals(new MultExpr(new VarExpr("y"), new VarExpr("y"))) );

    CHECK( (new MultExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(7))))
          ->subst("x", new VarExpr("y"))->equals(new MultExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(7)))));

    CHECK( (new VarExpr("x"))->subst("x", new VarExpr("7"))
          ->equals(new VarExpr("7")));

    CHECK( (new VarExpr("x"))->subst("x", new VarExpr("y"))
          ->equals(new VarExpr("y")));

    CHECK( (new AddExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(1))), new NumExpr(new NumVal(2))))
          ->subst("x", new NumExpr(new NumVal(7)))->equals( (new AddExpr(new AddExpr(new NumExpr(new NumVal(7)), new NumExpr(new NumVal(1))), new NumExpr(new NumVal(2))))));
}

TEST_CASE( "print" ) {
    
    {
    std::stringstream out("");
    (new NumExpr(new NumVal(7)))->print(out);
    CHECK( out.str() == "7");
    }
    
    {
    std::stringstream out("");
    (new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))))->print(out);
    CHECK( out.str() == "(1+2)");
    }
    
    {
    std::stringstream out("");
    (new AddExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(1))))->print(out);
    CHECK( out.str() == "((1+2)+1)");
    }
    
    {
    std::stringstream out("");
    (new MultExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(1))))->print(out);
    CHECK( out.str() == "((1+2)*1)");
    }
    
    {
    std::stringstream out("");
    (new MultExpr(new MultExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(1))))->print(out);
    CHECK( out.str() == "((1*2)*1)");
    }
    
    {
    std::stringstream out("");
    (new MultExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(1))))->print(out);
    CHECK( out.str() == "((x+2)*1)");
    }
    
    {
    std::stringstream out("");
    (new MultExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(2))), new VarExpr("y")))->print(out);
    CHECK( out.str() == "((x+2)*y)");
    }
}

TEST_CASE( "to_string" ) {
    CHECK( (new NumExpr(new NumVal(7)))->to_string() == "7");
    CHECK( (new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))))->to_string() == "1 + 2");
    CHECK( (new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(4))))->to_string() == "2 * 4");
    CHECK( (new VarExpr("x"))->to_string() == "x");
    CHECK( (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new LetExpr("y", new NumExpr(new NumVal(3)), new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(2)))), new VarExpr("x"))))->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE( "pretty_print_at" ) {
    CHECK( (new NumExpr(new NumVal(7)))->to_string() == "7");
    
    CHECK( (new AddExpr(new NumExpr(new NumVal(1)), new AddExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3)))))
          ->to_string() == "1 + 2 + 3");
    
    CHECK( (new AddExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(3))))
          ->to_string() == "(1 + 2) + 3");

    CHECK( (new AddExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new AddExpr(new NumExpr(new NumVal(3)), new NumExpr(new NumVal(4)))))->to_string() == "(1 + 2) + 3 + 4");
    
    CHECK( (new MultExpr(new NumExpr(new NumVal(2)), new MultExpr(new NumExpr(new NumVal(3)), new NumExpr(new NumVal(4)))))
          ->to_string() == "2 * 3 * 4");
    
    CHECK( (new MultExpr(new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3))), new NumExpr(new NumVal(4))))
          ->to_string() == "(2 * 3) * 4");
    
    CHECK( (new MultExpr(new MultExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new MultExpr(new NumExpr(new NumVal(3)), new NumExpr(new NumVal(4)))))->to_string() == "(1 * 2) * 3 * 4");
    
    CHECK( (new VarExpr("x"))->to_string() == "x");
    
    CHECK( (new AddExpr(new VarExpr("x"), new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2)))))->to_string() == "x + 1 + 2");
    
    CHECK( (new AddExpr(new AddExpr(new VarExpr("x"), new NumExpr(new NumVal(1))), new NumExpr(new NumVal(2))))->to_string() == "(x + 1) + 2");
    
    CHECK( (new AddExpr(new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3))), new AddExpr(new NumExpr(new NumVal(4)), new NumExpr(new NumVal(5)))))->to_string() == "2 * 3 + 4 + 5");
    
    CHECK( (new AddExpr(new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(1))))
          ->to_string() == "2 * 2 + 1");
        
    CHECK( (new AddExpr(new NumExpr(new NumVal(1)), new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(2)))))
          ->to_string() == "1 + 2 * 2");
    
    CHECK( (new AddExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new MultExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3)))))->to_string() == "(1 + 2) + 2 * 3");
    
    CHECK( (new AddExpr(new AddExpr(new NumExpr(new NumVal(2)), new NumExpr(new NumVal(3))), new MultExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2)))))->to_string() == "(2 + 3) + 1 * 2");
}

TEST_CASE( "pretty_print" ) {
    std::stringstream out("");
    (new VarExpr("x"))->pretty_print(out);
    CHECK( out.str() == "x");
    
    std::stringstream out2("");
    (new LetExpr("x", new NumExpr(new NumVal(5)), new AddExpr(new LetExpr("y", new NumExpr(new NumVal(3)), new AddExpr(new VarExpr("y"), new NumExpr(new NumVal(2)))), new VarExpr("x"))))->pretty_print(out2);
    CHECK( out2.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}
