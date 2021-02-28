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
#define Var Variable

// consumes char, checks if char consumed matches what is expected
static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}

void Expr::skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c)) {
            break;
        }
        consume(in, c);
    }
}

Expr* Expr::parse_expr(std::istream &in) {
    skip_whitespace(in);
    
    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return Expr::parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        Expr *e = Expr::parse_expr(in);
        Expr::skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing closing parenthesis");
        return e;
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

Expr* Expr::parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    
    while (1) {
        int c = in.peek();
        
        if (c == ')') {
            break;
        }
        
        c = in.get();
        if (isdigit(c))
            n = n*10 + (c - '0');
        else
            break;
    }
    
    if (negative) {
        n = -n;
    }
    
    return new Num(n);
}

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
    this->pretty_print(out);
    
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
    this->pretty_print(out);
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

//Expr* Add::parse(std::istream &in) {
//    return new Add(new Num(1), new Num(1));
//}
//Expr* Add::parse_str(std::string s) {
//    return new Add(new Num(1), new Num(1));
//}

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

Expr* Mult::subst(std::string string, Expr *replacement) {
    
    return new Mult(this->lhs->subst(string, replacement), this->rhs->subst(string, replacement));
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
    this->pretty_print(out);
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

//Expr* Mult::parse(std::istream &in) {
//    return new Mult(new Num(1), new Num(1));
//}
//Expr* Mult::parse_str(std::string s) {
//    return new Mult(new Num(1), new Num(1));
//}

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
    this->pretty_print(out);
    return out.str();
}

void Variable::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

//Expr* Variable::parse(std::istream &in) {
//    return new Variable("x");
//}
//Expr* Variable::parse_str(std::string s) {
//    return new Variable("x");
//}

/* Let Class *************************************************************/

Let::Let(std::string lhs, Expr * rhs, Expr * body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool Let::equals(Expr *o) {
    Let *c = dynamic_cast<Let*>(o);
    if (c == NULL) {
        return false;
    }
    else {
        return this->lhs == c->lhs &&  this->rhs->equals(c->rhs)
        && this->body->equals(c->body);
    }
}

int Let::interp() {
    
    this->rhs = new Num(rhs->interp());
    this->body = body->subst(lhs, rhs);
    
    return body->interp();
}

bool Let::has_variable() {
    return body->has_variable() || rhs->has_variable();
}

Expr* Let::subst(std::string string, Expr *e) {
    if (lhs != string) {
        return new Let(lhs, rhs->subst(string, e), body->subst(string, e));
    }
    return new Let(lhs, rhs->subst(string, e), body);
}

void Let::print(std::ostream& out) {
    out << "(_let ";
    out << lhs << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
}

std::string Let::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void Let::pretty_print_at(print_mode_t mode, std::ostream& out) {
    this->print(out);
}

//Expr* Let::parse(std::istream &in) {
//    return new Let("x", new Num(3), new Add(new Num(1), new Num(1)));
//}
//Expr* Let::parse_str(std::string s) {
//    return new Let("x", new Num(3), new Add(new Num(1), new Num(1)));
//}

/* TESTS *****************************************************************/

TEST_CASE( "Let" ) {
    
    // test constructor lhs
    CHECK( (new Let("x", new Num(5), new Add(new Variable("x"), new Num(5))))->lhs == "x");
    
    // test constructor rhs
    CHECK( (new Let("x", new Num(5), new Add(new Variable("x"), new Num(5))))->rhs->equals(new Num(5)));
    
    // test constructor body
    CHECK( (new Let("x", new Num(5), new Add(new Variable("x"), new Num(5))))->body->equals(new Add(new Variable("x"), new Num(5))));
    
    // test x is not used, expr == expr
    CHECK( (new Let("x", new Num(5), new Num(5)))->equals(new Let("x", new Num(5), new Num(5))));
    
    // exprs not equal
    CHECK( (new Let("x", new Num(6), new Add(new Variable("x"), new Num(3))))->equals(new Variable("x")) == false);
    
    // values in exprs not equal
    CHECK( (new Let("x", new Num(5), new Num(5)))->equals(new Let("y", new Num(5), new Num(5))) == false);
 
    // test subst
    Expr *let1 = (new Let("x",
                          new Num(1),
                          new Add(new Variable("x"), new Num(2))));
    CHECK( let1->subst("x", new Add(new Variable("y"), new Num(3)))
          ->equals(let1) );
    
    Expr *let2 = (new Let("x",
                          new Var("x"),
                          new Add(new Var("x"), new Num(2))));
    CHECK( let2->subst("x", new Add(new Var("y"), new Num(3)))
          ->equals(new Let("x",
                           new Add(new Var("y"), new Num(3)),
                           new Add(new Var("x"), new Num(2)))) );
    
    CHECK( (new Let("x", new Variable("y"), new Add(new Variable("x"), new Num(2))))->subst("y", new Num(8))->equals(new Let("x", new Num(8), new Add(new Variable("x"), new Num(2)))));
    
    CHECK( (new Let("x", new Num(8), new Add(new Add(new Variable("x"), new Num(2)), new Variable("y"))))->subst("y", new Num(9))
          ->equals(new Let("x", new Num(8), new Add(new Add(new Variable("x"), new Num(2)), new Num(9)))));
    
    CHECK( (new Add(new Variable("y"), new Variable("y")))->subst("y", new Num(8))->equals(new Add(new Num(8), new Num(8))));
    
    CHECK( (new Let("x", new Variable("y"), new Add(new Variable("x"), new Variable("y"))))->subst("y", new Num(8))->equals(new Let("x", new Num(8), new Add(new Variable("x"), new Num(8)))));
    
    // test doesn't replace variable on lhs in Let
    CHECK( (new Let("z", new Variable("x"), new Add(new Variable("z"), new Num(32))))->subst("z", new Num(0))->equals(new Let("z", new Variable("x"), new Add(new Variable("z"), new Num(32)))));
    
    CHECK( (new Let("z", new Variable("z"), new Add(new Variable("z"), new Num(32))))->subst("z", new Num(0))->equals(new Let("z", new Num(0), new Add(new Variable("z"), new Num(32)))));
    
    CHECK( (new Let("z", new Add(new Variable("z"), new Num(2)), new Add(new Variable("z"), new Num(32))))->subst("z", new Num(0))->equals(new Let("z", new Add(new Num(0), new Num(2)), new Add(new Variable("z"), new Num(32)))));
    
    // test has_variable
    CHECK( (new Let("x", new Num(5), new Add(new Variable("x"), new Num(5))))->has_variable());
    
    CHECK( (new Let("x", new Variable("y"), new Add(new Num(2), new Num(5))))->has_variable());
    
    CHECK( (new Let("x", new Num(2), new Add(new Num(2), new Num(5))))->has_variable() == false);
    
    CHECK( (new Let("x", new Variable("z"), new Add(new Variable("x"), new Num(5))))->has_variable());
    
    // test interp
    CHECK( (new Let("x", new Num(2), new Add(new Variable("x"), new Num(32))))->interp() == 34);
    
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
    CHECK( (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Variable("y"), new Num(2))), new Variable("x"))))->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
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

TEST_CASE( "pretty_print" ) {
    std::stringstream out("");
    (new Variable("x"))->pretty_print(out);
    CHECK( out.str() == "x");
    
    std::stringstream out2("");
    (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Variable("y"), new Num(2))), new Variable("x"))))->pretty_print(out2);
    CHECK( out2.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE( "parse" ) {
    
}












