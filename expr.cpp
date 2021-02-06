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

void Expr::pretty_print(std::ostream& out) {
    this->pretty_print_at(print_group_none, out);
}

// Num Constructor implementation
    Num::Num(int val) {
    this->val = val;
}

// Num Equals implementation
bool Num::equals(Expr *o) {
    Num *c = dynamic_cast<Num*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->val == c->val);
    }
}

// Num Interp implementation
int Num::interp() {
    return this->val;
}

// Num has_variable implementation
bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string string, Expr *e) {
    return this;
}

void Num::print(std::ostream &out) {
    out << this->val;
}

std::string Num::to_string() {
    
    std::stringstream out("");
    this->pretty_print_at(print_group_none, out);
    
    return out.str();
}

void Num::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

// Add Constructor implementation
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Add Equals implementation
bool Add::equals(Expr *o) {
    Add *c = dynamic_cast<Add*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs));
    }
}

// Add Interp implementation
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

// Add has_variable implementation
bool Add::has_variable() {
    
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Add::subst(std::string string, Expr *exprSub) {
    
    return new Add(this->lhs->subst(string, exprSub), this->rhs->subst(string, exprSub));

}

void Add::print(std::ostream &out) {
    out << '(';
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ')';
}

std::string Add::to_string() {
    
    std::stringstream out("");
    this->pretty_print_at(print_group_none, out);
    return out.str();
    
}

void Add::pretty_print_at(print_mode_t mode, std::ostream& out) {
    
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
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Mult Equals implementation
bool Mult::equals(Expr *o) {
    Mult *c = dynamic_cast<Mult*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return ((this->lhs->equals(c->lhs)) && (this->rhs->equals(c->rhs) ));
    }
}

// Mult Interp implementation
int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

// Mult has_variable implementation
bool Mult::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Mult::subst(std::string string, Expr *exprSub) {
    
    return new Mult(this->lhs->subst(string, exprSub), this->rhs->subst(string, exprSub));
}

void Mult::print(std::ostream &out) {
    out << '(';
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ')';
}

std::string Mult::to_string() {
    std::stringstream out("");
    this->pretty_print_at(print_group_none, out);
    return out.str();
}

void Mult::pretty_print_at(print_mode_t mode, std::ostream& out) {
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
Variable::Variable(std::string string) {
    this->string = string;
}

// Variable Equals implementation
bool Variable::equals(Expr *o) {
    Variable *c = dynamic_cast<Variable*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return (this->string == c->string);
    }
}

// Variable Interp implementation
int Variable::interp() {
    
    throw std::runtime_error("no value for variable");
    
}

// Variable has_variable implementation
bool Variable::has_variable() {
    return true;
}

Expr* Variable::subst(std::string string, Expr *replacement) {

    if (this->string == string) {
        return replacement;
    }
    
    return this;
}

void Variable::print(std::ostream &out) {
    out << this->string;
}

std::string Variable::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void Variable::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

// test constructor and equals implementations
TEST_CASE("Num") {
    
    CHECK( (new Num(7))->equals(new Variable("x")) == false);
    
    Num *c = new Num(3);
    Num *b = new Num(3);
    Num *a = new Num(5);
    CHECK(c->val == 3);
    CHECK(a->val == 5);
    CHECK(c->equals(b));
    CHECK(c->equals(a) == false);
}

// test constructor and equals implementations
TEST_CASE("Add") {
    Num *a = new Num(3);
    Num *b = new Num(5);
    Add *expr1 = new Add(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);
    CHECK(expr1->equals(a) == false);

    Add *expr2 = new Add(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    Add *expr3 = new Add(a, b);
    CHECK(expr1->equals(expr3));
}

// test constructor and equals implementations
TEST_CASE("Mult") {
    Num *a = new Num(3);
    Num *b = new Num(5);
    Mult *expr1 = new Mult(a, b);
    CHECK(expr1->lhs->equals(a));
    CHECK(expr1->rhs->equals(b));
    CHECK(expr1->lhs->equals(b) == false);

    Mult *expr2 = new Mult(b, a);
    CHECK(expr1->equals(expr1));
    CHECK(expr1->equals(expr2) == false);
    Mult *expr3 = new Mult(a, b);
    CHECK(expr1->equals(expr3));
    
    CHECK( (new Mult(new Num(2), new Num(3)))
          ->equals(new Variable("x")) == false);
}

TEST_CASE("Variable") {
    Variable *a = new Variable("cat");
    Variable *b = new Variable("b");
    CHECK(a->string == "cat");
    CHECK(b->string == "b");

    Variable *c = new Variable("cat");
    CHECK(a->equals(c));
    CHECK(a->equals(b) == false);

    Num *one = new Num(10);
    CHECK(a->equals(one) == false);
    
    // Wanted to double check variable->equals was working
    CHECK( (new Variable("x"))->equals(new Variable("x")));
    CHECK( (new Variable("x"))->equals(new Variable("y")) == false);
}

TEST_CASE( "Interp" ) {
    CHECK( (new Num(4))->interp() == 4);
    CHECK( (new Num(4))->interp() != 3);
    
    CHECK( (new Add(new Num(4), new Num(3)))->interp() == 7);
    CHECK( (new Add(new Num(4), new Num(3)))->interp() != 6);
    
    CHECK( (new Mult(new Num(4), new Num(3)))->interp() == 12);
    
    CHECK_THROWS_WITH( (new Variable("x"))->interp(), "no value for variable" );
}

TEST_CASE( "has_variable" ) {
    CHECK( (new Num(4))->has_variable() == false);
    
    CHECK( (new Add(new Num(4), new Num(3)))->has_variable() == false);
    CHECK( (new Add(new Variable("x"), new Num(4)))->has_variable() == true);
    CHECK( (new Add(new Num(2), new Variable("x")))->has_variable() == true);
    CHECK( (new Add(new Variable("x"), new Variable("y")))->has_variable() == true);
    
    CHECK( (new Mult(new Num(4), new Num(8)))->has_variable() == false);
    CHECK( (new Mult(new Variable("x"), new Num(3)))->has_variable() == true);
    CHECK( (new Mult(new Num(2), new Variable("x")))->has_variable() == true);
    CHECK( (new Mult(new Variable("x"), new Variable("y")))->has_variable() == true);
}

TEST_CASE( "subst" ) {
    
    CHECK( (new Num(8))->subst("x", new Variable("y"))->equals(new Num(8)));
    
    CHECK( (new Variable("x"))->subst("x", new Variable("y"))
          ->equals(new Variable("y")));
    
    CHECK( (new Variable("x"))->subst("z", new Variable("y"))
          ->equals(new Variable("x")));
    
    CHECK( (new Add(new Variable("x"), new Num(7)))
           ->subst("x", new Variable("y"))
           ->equals(new Add(new Variable("y"), new Num(7))) );
    
    CHECK( (new Add(new Num(7), new Variable("x")))
           ->subst("x", new Variable("y"))
           ->equals(new Add(new Num(7), new Variable("y"))) );

    CHECK( (new Add(new Variable("x"), new Variable("x")))
           ->subst("x", new Variable("y"))
           ->equals(new Add(new Variable("y"), new Variable("y"))) );

    CHECK( (new Add(new Num(4), new Num(7)))
          ->subst("x", new Variable("y"))
          ->equals(new Add(new Num(4), new Num(7))));

    CHECK( (new Mult(new Variable("x"), new Num(7)))
           ->subst("x", new Variable("y"))
           ->equals(new Mult(new Variable("y"), new Num(7))) );

    CHECK( (new Mult(new Num(7), new Variable("x")))
           ->subst("x", new Variable("y"))
           ->equals(new Mult(new Num(7), new Variable("y"))) );

    CHECK( (new Mult(new Variable("x"), new Variable("x")))
           ->subst("x", new Variable("y"))
           ->equals(new Mult(new Variable("y"), new Variable("y"))) );

    CHECK( (new Mult(new Num(4), new Num(7)))
          ->subst("x", new Variable("y"))->equals(new Mult(new Num(4), new Num(7))));

    CHECK( (new Variable("x"))->subst("x", new Variable("7"))
          ->equals(new Variable("7")));

    CHECK( (new Variable("x"))->subst("x", new Variable("y"))
          ->equals(new Variable("y")));

    CHECK( (new Add(new Add(new Variable("x"), new Num(1)), new Num(2)))
          ->subst("x", new Num(7))->equals( (new Add(new Add(new Num(7), new Num(1)), new Num(2)))));
}

TEST_CASE( "print" ) {
    
    {
    std::stringstream out("");
    (new Num(7))->print(out);
    CHECK( out.str() == "7");
    }
    
    {
    std::stringstream out("");
    (new Add(new Num(1), new Num(2)))->print(out);
    CHECK( out.str() == "(1+2)");
    }
    
    {
    std::stringstream out("");
    (new Add(new Add(new Num(1), new Num(2)), new Num(1)))->print(out);
    CHECK( out.str() == "((1+2)+1)");
    }
    
    {
    std::stringstream out("");
    (new Mult(new Add(new Num(1), new Num(2)), new Num(1)))->print(out);
    CHECK( out.str() == "((1+2)*1)");
    }
    
    {
    std::stringstream out("");
    (new Mult(new Mult(new Num(1), new Num(2)), new Num(1)))->print(out);
    CHECK( out.str() == "((1*2)*1)");
    }
    
    {
    std::stringstream out("");
    (new Mult(new Add(new Variable("x"), new Num(2)), new Num(1)))->print(out);
    CHECK( out.str() == "((x+2)*1)");
    }
    
    {
    std::stringstream out("");
    (new Mult(new Add(new Variable("x"), new Num(2)), new Variable("y")))->print(out);
    CHECK( out.str() == "((x+2)*y)");
    }
}

TEST_CASE( "to_string" ) {
    CHECK( (new Num(7))->to_string() == "7");
    CHECK( (new Add(new Num(1), new Num(2)))->to_string() == "1 + 2");
    CHECK( (new Mult(new Num(2), new Num(4)))->to_string() == "2 * 4");
    CHECK( (new Variable("x"))->to_string() == "x");
}

TEST_CASE( "pretty_print_at" ) {
    CHECK( (new Num(7))->to_string() == "7");
    
    CHECK( (new Add(new Num(1), new Add(new Num(2), new Num(3))))
          ->to_string() == "1 + 2 + 3");
    
    CHECK( (new Add(new Add(new Num(1), new Num(2)), new Num(3)))
          ->to_string() == "(1 + 2) + 3");

    CHECK( (new Add(new Add(new Num(1), new Num(2)), new Add(new Num(3), new Num(4))))->to_string() == "(1 + 2) + 3 + 4");
    
    CHECK( (new Mult(new Num(2), new Mult(new Num(3), new Num(4))))
          ->to_string() == "2 * 3 * 4");
    
    CHECK( (new Mult(new Mult(new Num(2), new Num(3)), new Num(4)))
          ->to_string() == "(2 * 3) * 4");
    
    CHECK( (new Mult(new Mult(new Num(1), new Num(2)), new Mult(new Num(3), new Num(4))))->to_string() == "(1 * 2) * 3 * 4");
    
    CHECK( (new Variable("x"))->to_string() == "x");
    
    CHECK( (new Add(new Variable("x"), new Add(new Num(1), new Num(2))))->to_string() == "x + 1 + 2");
    
    CHECK( (new Add(new Add(new Variable("x"), new Num(1)), new Num(2)))->to_string() == "(x + 1) + 2");
    
    CHECK( (new Add(new Mult(new Num(2), new Num(3)), new Add(new Num(4), new Num(5))))->to_string() == "2 * 3 + 4 + 5");
    
    CHECK( (new Add(new Mult(new Num(2), new Num(2)), new Num(1)))
          ->to_string() == "2 * 2 + 1");
        
    CHECK( (new Add(new Num(1), new Mult(new Num(2), new Num(2))))
          ->to_string() == "1 + 2 * 2");
    
    CHECK( (new Add(new Add(new Num(1), new Num(2)), new Mult(new Num(2), new Num(3))))->to_string() == "(1 + 2) + 2 * 3");
    
    CHECK( (new Add(new Add(new Num(2), new Num(3)), new Mult(new Num(1), new Num(2))))->to_string() == "(2 + 3) + 1 * 2");
}












