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
#include "pointer.h"

class Expr;

class Parse {
public:
    
    static PTR(Expr) parse_num(std::istream &in);
    static PTR(Expr) parse_comparg(std::istream &in);
    static PTR(Expr) parse_expr(std::istream &in);
    static void skip_whitespace(std::istream &in);
    static PTR(Expr) parse_inner(std::istream &in);
    static PTR(Expr) parse_multicand(std::istream &in);
    static PTR(Expr) parse_addend(std::istream &in);
    static PTR(Expr) parse_let(std::istream &in);
};

#endif /* parse_hpp */
