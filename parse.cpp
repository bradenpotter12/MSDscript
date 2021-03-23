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
        
        if (c == ')' || c == '+' || c == '*' || c == '=') {
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

// <expr> = <comparg> | <comparg> == <expr>
Expr* Parse::parse_expr(std::istream &in) {
    Expr *e;
    
    e = Parse::parse_comparg(in);
    
    Parse::skip_whitespace(in);
    
    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        consume(in, '=');
        Expr *rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    } else
        return e;
}

TEST_CASE( "parse_expr" ) {
    CHECK( (new EqExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(1))))->to_string() == "(1==1)");
    
    CHECK( parse_str("1+2==3")->to_string() == "((1+2)==3)");
    CHECK( parse_str("1+2==3")->equals(new EqExpr(new AddExpr(new NumExpr(new NumVal(1)), new NumExpr(new NumVal(2))), new NumExpr(new NumVal(3)))));
    
    CHECK( (parse_str("1 + 2")->interp()->equals(new NumVal(3))));
    CHECK( (parse_str("1+2==3"))->interp()->equals(new BoolVal(true)));
    CHECK( (parse_str("1+2==2"))->interp()->equals(new BoolVal(false)));
}

// <comparg> = <addend> | <addend> + <comparg>
Expr* Parse::parse_comparg(std::istream &in) {
    Expr *e;
    
    e = Parse::parse_addend(in);
    
    Parse::skip_whitespace(in);
    
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

std::string parse_keyword(std::istream &in) {
    
    consume(in, '_'); // consume '_'
    int next_ch = in.peek();
    
    if (next_ch == 'l') {

        return "_let";
    }
    
    else if (next_ch == 'i') {

        return "_if";
    }
    
    else if (next_ch == 't') {

        return "_true";
    }
    
    else if (next_ch == 'f') {
        consume(in, 'f');
        int c = in.get();
        
        if (c == 'u')
            return "_fun";

        return "_false";
    }
    
    throw std::runtime_error("invalid input");
}

Expr* parse_if(std::istream &in) {
    
    std::string str = "if";
    for (int i = 0; i < str.length(); i++) {
        consume(in, str[i]);
    }
    
    Expr* ifExpr = Parse::parse_expr(in);
    
    Parse::skip_whitespace(in);
    
    std::string str2 = "_then";
    for (int i = 0; i < str2.length(); i++) {
        consume(in, str2[i]);
    }
    
    Expr* thenExpr = Parse::parse_expr(in);
    
    Parse::skip_whitespace(in);
    
    std::string str3 = "_else";
    for (int i = 0; i < str3.length(); i++) {
        consume(in, str3[i]);
    }
    
    Expr* elseExpr = Parse::parse_expr(in);
    
    return new IfExpr(ifExpr, thenExpr, elseExpr);
    
}

TEST_CASE( "parse_if" ) {
    
    CHECK( (parse_str("_if _true _then _false _else _true"))->interp()->equals(new BoolVal(false)));
    
    CHECK( (parse_str("_if _false _then _false _else _true"))->interp()->equals(new BoolVal(true)));
    
    CHECK( (parse_str("_if 1 == 1 _then _false _else _true"))->interp()->equals(new BoolVal(false)));
    
    CHECK( (parse_str("_if 1 == 2 _then _false _else _true"))->interp()->equals(new BoolVal(true)));
}

// <inner> | <multicand>(<expr>)
Expr* Parse::parse_multicand(std::istream &in) {
    Expr *expr = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        consume(in, ')');
        expr = new CallExpr(expr, actual_arg);
    }
    return expr;
}

Expr* parse_fun(std::istream &in) {
    
    consume(in, 'n');
    Parse::skip_whitespace(in);
    consume(in, '(');
    
    char next_ch = in.get(); // get formal_arg
    std::string formal_arg;
    formal_arg = next_ch;
    
    // make sure formal_arg isn't more than one letter
    while (isalpha(in.peek())) {
        
        char c = in.get();
        formal_arg += c;
    }
    
    consume(in, ')');
    Parse::skip_whitespace(in);
    
    Expr *body = Parse::parse_expr(in);
    
    return new FunExpr(formal_arg, body);
}

TEST_CASE( "parse_fun" ) {
    CHECK( (parse_str("(_fun (x) x*3)(10)"))->interp()->equals(new NumVal(30)));
    CHECK( (parse_str("_fun (money) money*money"))->to_string() == "_fun (money) (money*money)");
    CHECK( (parse_str("_let x = 3 _in x*x"))->interp()->to_string() == "9");
    CHECK( (parse_str("_let x = _fun (x) x*x _in x"))->interp()->to_string() == "_fun (x) (x*x)");
    CHECK( (parse_str("_let f = _fun (x) x + 1 _in f(10)"))->interp()->to_string() == "11");
}

// <expr> = <number> | (<expr>)
Expr* Parse::parse_inner(std::istream &in) {
    skip_whitespace(in);
    
    int c = in.peek();
    
    // <number>
    if ((c == '-') || isdigit(c))
        return Parse::parse_num(in);
    
    // (<expr>)
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_comparg(in);
        Parse::skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing closing parenthesis");
        return e;
    }
        
    // _let <variable> = <expr> _in <expr>
    else if (c == '_') {
        
        std::string kw = parse_keyword(in);
        
        if (kw == "_let")
            return parse_let(in);
        
        else if (kw == "_true") {
            
            std::string str = "true";
            for (int i = 0; i < str.length(); i++) {
                consume(in, str[i]);
            }
            
            return new BoolExpr(true);
        }
        else if (kw == "_false") {
            
            std::string str = "lse";
            for (int i = 0; i < str.length(); i++) {
                consume(in, str[i]);
            }
            
            return new BoolExpr(false);
        }
        else if (kw == "_if") {
            return parse_if(in);
        }
        else if (kw == "_fun")
            return parse_fun(in);
        
        throw std::runtime_error("invalid input");
    }
    
    // <variable>
    else if (isalpha(c)) {
        
        char c = in.get();
        std::string var;
        var += c;
        
        // make sure variable isn't more than one letter
        while (isalpha(in.peek())) {
            
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

TEST_CASE( "parse_inner" ) {
    CHECK( (parse_str("_false"))->equals(new BoolExpr(false)));
    CHECK( (parse_str("_true"))->equals(new BoolExpr(true)));
    CHECK_THROWS_WITH(parse_str("_r"), "invalid input");
}

Expr* Parse::parse_let(std::istream &in) {
    std::string lhs;
    Expr* rhs = nullptr;
    Expr* body = nullptr;
    bool body_parsed = false;
    
    while (1) {
        
        int next_ch = in.peek();
        
        if (next_ch == 'l') {
            
            std::string str = "let";
            for (int i = 0; i < str.length(); i++) {
                consume(in, str[i]);
            }
            
            Parse::skip_whitespace(in);
            
            lhs = in.get();
            
            // make sure variable isn't more than one letter
            while (in.peek() != ' ' && in.peek() != '=') {
                
                char c = in.get();
                lhs += c;
            }
            
            skip_whitespace(in);
            int c = in.get();
            if (c != '=') {
                throw std::runtime_error("invalid input");
            }
            skip_whitespace(in);
            rhs = parse_comparg(in);
        }
        else if (next_ch == '_') {
            
            std::string str = "_in";
            for (int i = 0; i < str.length(); i++) {
                consume(in, str[i]);
            }
            
            Parse::skip_whitespace(in);
            
            body = parse_comparg(in);
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
    CHECK_THROWS_WITH( Parse::parse_expr(in), "invalid input");
    }
    
    std::stringstream in("_let x *");
    CHECK_THROWS_WITH( Parse::parse_expr(in), "invalid input");
    
    CHECK_THROWS_WITH( parse_str("_l$et"), "consume mismatch");
}
