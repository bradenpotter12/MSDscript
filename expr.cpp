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
    
    return 0;
}

// Variable has_variable implementation
bool Variable::has_variable() {
    return true;
}

Expr* Variable::subst(std::string string, Expr *exprSub) {

    if (this->string == string) {
        return exprSub;
    }
    
    return this;
}

void Variable::print(std::ostream &out) {
    out << this->string;
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





