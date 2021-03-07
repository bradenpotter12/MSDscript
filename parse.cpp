//
//  parse.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 3/7/21.
//

#include "parse.hpp"
#include "catch.hpp"
#include "val.hpp"
#include "expr.hpp"

// Parse wrapper for testing
Expr *parse_str(std::string s) {
    std::stringstream in(s);
    return Parse::parse_expr(in);
}

// consumes char, checks if char consumed matches what is expected
static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}

void Parse::skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c)) {
            break;
        }
        consume(in, c);
    }
}

Expr* Parse::parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    
    while (1) {
        int c = in.peek();
        
        if (c == ')' || c == '+' || c == '*') {
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
   
    return (new NumVal(n))->to_expr();
}

// <addend> = <multicand> | <multicand> * <addend>
Expr* Parse::parse_addend(std::istream &in) {
    Expr *e;
    
    e = Parse::parse_multicand(in);
    
    Parse::skip_whitespace(in);
    
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    } else
        return e;
}

// <expr> = <addend> | <addend> + <expr>
Expr* Parse::parse_expr(std::istream &in) {
    Expr *e;
    
    e = Parse::parse_addend(in);
    
    Parse::skip_whitespace(in);
    
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

// <expr> = <number> | (<expr>)
Expr* Parse::parse_multicand(std::istream &in) {
    skip_whitespace(in);
    
    int c = in.peek();
    
    // <number>
    if ((c == '-') || isdigit(c))
        return Parse::parse_num(in);
    
    // (<expr>)
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        Parse::skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing closing parenthesis");
        return e;
    }
        
    // _let <variable> = <expr> _in <expr>
    else if (c == '_') {
    
        Expr *e = parse_let(in);
        return e;
    }
    
    // <variable>
    else if (isalpha(c)) {
        
        char c = in.get();
        std::string var;
        var += c;
        
        // make sure variable isn't more than one letter
        while (in.peek() != ' ' && in.peek() != -1) {
            char c = in.get();
            var += c;
        }
        return new VarExpr(var);
    }
        
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

std::string parse_keyword(std::istream &in) {
    
    int c = in.peek();
    
    if (c == '_') {
        
        consume(in, '_'); // consume '_'
        int char1 = in.peek();
        
        if (char1 == 'l') {
            std::string str = "let ";
            for (int i = 0; i < str.length(); i++) { // HERE
                consume(in, str[i]);
            }
            return "_let";
        }
            
        else if (char1 == 'i') {
            std::string str = "in ";
            for (int i = 0; i < str.length(); i++) {
                consume(in, str[i]);
            }
            return "_in";
        }
    }
    throw std::runtime_error("invalid input");
}

Expr* Parse::parse_let(std::istream &in) {
    std::string lhs;
    Expr* rhs = nullptr;
    Expr* body = nullptr;
    bool body_parsed = false;
    
    while (1) {
        std::string keyword = parse_keyword(in);
        
        if (keyword == "_let") {
            skip_whitespace(in);
            
            lhs = in.get();
            
            // make sure variable isn't more than one letter
            while (in.peek() != ' ') {
                char c = in.get();
                lhs += c;
            }
            
            skip_whitespace(in);
            int c = in.get();
            if (c != '=') {
                throw std::runtime_error("invalid input");
            }
            skip_whitespace(in);
            rhs = parse_expr(in);
        }
        else if (keyword == "_in") {
            skip_whitespace(in);
            body = parse_expr(in);
            body_parsed = true;
        }
        
        if (body_parsed) {
            return new LetExpr(lhs, rhs, body);
        }
    }
}

TEST_CASE( "parse" ) {
    
    CHECK( (parse_str("x"))->to_string() == "x");
    CHECK_THROWS_WITH( (parse_str("x"))->interp(), "no value for variable");
    CHECK( (parse_str("money"))->to_string() == "money");
    CHECK( (parse_str("1"))->to_string() == "1");
    CHECK( (parse_str("1"))->interp()->equals(new NumVal(1)));
    
    CHECK( (parse_str("x + 2"))->to_string() == "x + 2");
    CHECK( (parse_str("year + 2"))->to_string() == "year + 2");
    CHECK( (parse_str("x + (-2)"))->to_string() == "x + -2");
    CHECK( (parse_str("(1 + 2)*3"))->to_string() == "(1 + 2) * 3");
    CHECK( (parse_str("(1+2)*3"))->to_string() == "(1 + 2) * 3");
    CHECK( (parse_str("(2*2)*3"))->to_string() == "(2 * 2) * 3");
    CHECK( (parse_str("(2*2)+2*3"))->to_string() == "2 * 2 + 2 * 3");
    
    CHECK( (parse_str("2 + (-2)"))->interp()->equals(new NumVal(0)));
    CHECK( (parse_str("2 * 2 + 3"))->interp()->equals(new NumVal(7)));
    CHECK( (parse_str("(2 + 2) * 3"))->interp()->equals(new NumVal(12)));
    CHECK( (parse_str("(-2 + 2) * 3"))->interp()->equals(new NumVal(0)));
    CHECK( (parse_str("(2 + 2) * (-3)"))->interp()->equals(new NumVal(-12)));
    
    CHECK( (parse_str("_let x = 2 _in x + 2"))->interp()->equals(new NumVal(4)));
    CHECK( (parse_str("_let x = 2 _in x"))->interp()->equals(new NumVal(2)));
    CHECK( (parse_str("_let x = 2 + 3 _in x"))->interp()->equals(new NumVal(5)));
    CHECK( (parse_str("_let x = 2 + 3 _in x + 3"))->interp()->equals(new NumVal(8)));
    CHECK( (parse_str("_let year = 2 _in year + 2"))->interp()->equals(new NumVal(4)));
    
    CHECK( (parse_str("_let x = 2 _in x + 2"))->to_string()
          == "(_let x=2 _in (x+2))");
    CHECK( (parse_str("_let year = 2 _in year + 2"))->to_string()
          == "(_let year=2 _in (year+2))");
    
    CHECK( (parse_str("2 * (_let x = 2 + 3 _in x + 3)"))->interp()->equals(new NumVal(16)));
    
    CHECK( (parse_str("2 + (2 * (_let x = 2 + 3 _in x + 3))"))->interp()->equals(new NumVal(18)));
    
    CHECK( (parse_str("_let x = 2 + (2 * (_let x = 2 + 3 _in x + 3)) _in x + 2"))
          ->interp()->equals(new NumVal(20)));
          
    CHECK( (parse_str("_let x = 2 + (2 * (_let x = 2 + 3 _in x + 3)) _in x * 2"))
          ->interp()->equals(new NumVal(36)));
    
    CHECK( (parse_str("_let var = _let x = 2 + (2 * (_let x = 2 + 3 _in x + 3)) _in x * 2 _in _let x = 4 _in var + x"))->interp()->equals(new NumVal(40)));
    
    CHECK_THROWS_WITH( parse_str("(1"), "missing closing parenthesis" );
    CHECK_THROWS_WITH( parse_str("$ x = 1 _in x + 1"), "invalid input");
    {
    std::stringstream in('$');
    CHECK_THROWS_WITH( parse_keyword(in), "invalid input");
    }
    
    std::stringstream in("_let x *");
    CHECK_THROWS_WITH( Parse::parse_let(in), "invalid input");
    CHECK_THROWS_WITH( parse_str("_l$et"), "consume mismatch");
}
