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



Expr *parse_string(std::string s) {
    std::stringstream in(s);
    return Expr::parse_expr(in);
}

int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    
//    std::string string = "2 * 2 + _let x = 3 _in x + 3";
//    std::string str = "";
//
//    Expr *e = parse_string(string);
//    Val *o = e->interp();
//
//    std::cout << o->interp() << "\n";
    
    return 0;
}


