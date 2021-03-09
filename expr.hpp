#include <string>
#include <stdio.h>
#include <sstream>

class Val;

// Expr Class
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    
    // Returns an int for the value of an expression
    virtual Val* interp() = 0;
    
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
    
};

// Num Class
class NumExpr : public Expr {
public:
    Val *val;
    
    // constructor
    NumExpr(Val *val);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Add Class
class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    AddExpr(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Mult Class
class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    
    // constructor
    MultExpr(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// Variable Class
class VarExpr : public Expr {
public:
    std::string string;
    
    // constructor
    VarExpr(std::string string);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

// _let Class
class LetExpr : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;
    
    LetExpr(std::string lhs, Expr *rhs, Expr *body);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    
};

class EqExpr : public Expr {
public:
    
    Expr *lhs;
    Expr *rhs;
    
    EqExpr(Expr *lhs, Expr *rhs);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
};

class BoolExpr : public Expr {
public:
    
    Val* boolVal;
    
    BoolExpr(bool val);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
};

class IfExpr : public Expr {
public:
    
    Expr *ifExpr;
    Expr *thenExpr;
    Expr *elseExpr;
    
    IfExpr(Expr *ifExpr, Expr *thenExpr, Expr *elseExpr);
    
    bool equals(Expr *o);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string string, Expr *replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
};

