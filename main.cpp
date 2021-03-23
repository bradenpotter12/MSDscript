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
    
//    std::string string = "2 * 2 + _let x = 3 _in x + 3";
//    std::string str = "";
//
//    PTR(Expr) e = parse_string(string);
//    PTR(Val) o = e->interp();
//
//    std::cout << o->to_string() << "\n";
//    
//    std::cout << (NEW(EqExpr) (NEW(BoolExpr)(true), NEW(BoolExpr)(true)))->to_string() << "\n";
//
//    PTR(Expr) expr = parse_string("(_fun (x) x*x)(4)");
//
//    std::cout << expr->interp()->to_string() << "\n";
    
    return 0;
}


