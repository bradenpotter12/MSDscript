//
//  main.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#include <iostream>
#include "cmdline.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "expr.hpp"

TEST_CASE("Num") {
    Num *c = new Num(3);
    Num *b = new Num(3);
    Num *a = new Num(5);
    CHECK(c->val == 3);
    CHECK(c->equals(b));
    CHECK(c->equals(a) == false);
}

TEST_CASE("Add") {
    
}


int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    Catch::Session().run();
    
    return 0;
}
