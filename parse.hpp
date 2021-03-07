//
//  parse.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/7/21.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include <sstream>

class Expr;

class Parse {
public:
    
    static Expr* parse_num(std::istream &in);
    static Expr* parse_expr(std::istream &in);
    static void skip_whitespace(std::istream &in);
    static Expr* parse_multicand(std::istream &in);
    static Expr* parse_addend(std::istream &in);
    static Expr* parse_let(std::istream &in);
};

#endif /* parse_hpp */
