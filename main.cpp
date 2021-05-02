//
//  main.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//


#include <iostream>
#include "cmdline.hpp"
#include "expr.hpp"
#include "catch.hpp"
#include "val.hpp"
#include "parse.hpp"



PTR(Expr) parse_string(std::string s) {
    std::stringstream in(s);
    return Parse::parse_expr(in);
}

int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    
    
    
    return 0;
}


