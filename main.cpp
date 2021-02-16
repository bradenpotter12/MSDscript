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


int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    
    std::stringstream out("");
    (new Add(new Add(new Num(1), new Num(2)), new Num(3)))->pretty_print_at(Expr::print_group_none, out);

    std::cout << out.str() << "\n";
    
    std::stringstream out2("");
    (new Add(new Num(1), new Add(new Num(2), new Num(3))))->pretty_print_at(Expr::print_group_none, out2);
    
    std::cout << out2.str() << "\n";
    
    std::stringstream out3("");
    (new Mult(new Num(2), new Mult(new Num(3), new Num(4))))
          ->pretty_print_at(Expr::print_group_none, out3);
    std::cout << out3.str() << "\n";

    std::stringstream letOut("");
    (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Variable("y"), new Num(2))), new Variable("x"))))->print(letOut);
    std::cout << letOut.str() << "\n";
    
    return 0;
}


