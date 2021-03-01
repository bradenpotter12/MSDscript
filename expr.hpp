#include <string>
#include <stdio.h>
#include <sstream>

// Expr Class
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    
    // Returns an int for the value of an expression
    virtual int interp() = 0;
    
    // Returns true if the expression is a variable or contains
    // a variable, false otherwise
    virtual bool has_variable() = 0;
    
    // If an expression has a variable that matches the string
    // parameter when subst() is called, it will replace the
    // variable in the expression with a different variable
    virtual Expr* subst(std::string string, Expr *e) = 0;
    
    typedef enum {
        print_group_none,
        print_group_add,
        print_group_add_or_mult
    }   print_mode_t;
    
    virtual void print(std::ostream& out) = 0;
    virtual std::string to_string() = 0;
    void pretty_print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out) = 0;
    
    static Expr* parse_num(std::istream &in);
    static Expr* parse_expr(std::istream &in);
    static Expr* parse_expr2(std::istream &in);
    static void skip_whitespace(std::istream &in);
    static Expr* parse_multicand(std::istream &in);
    static Expr* parse_addend(std::istream &in);
//    Expr* parse_str(std::string s);
};

// Num Class
class Num : public Expr {
public:
    int val;
    
    // constructor
    Num(int val);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Add Class
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Add(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Mult Class
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    Mult(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Variable Class
class Variable : public Expr {
public:
    std::string string;
    
    // constructor
    Variable(std::string string);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// _let Class
class Let : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;
    
    Let(std::string lhs, Expr *rhs, Expr *body);
    
    bool equals(Expr *o);
    int interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};
