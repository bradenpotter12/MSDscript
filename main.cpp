//
//  main.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#include <iostream>
#include "cmdline.hpp"
#include "expr.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

// test constructor and equals implementations
TEST_CASE("Num") {
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
}

int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    Catch::Session().run();
    
    return 0;
}
