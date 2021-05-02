#include <string>
#include <stdio.h>
#include <sstream>
#include "pointer.h"

class Env;
class Val;

class Expr : public std::enable_shared_from_this<Expr> {
public:
    virtual bool equals(PTR(Expr)) = 0;
    
    // Returns an int for the value of an expression
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    
    // If an expression has a variable that matches the string
    // parameter when subst() is called, it will replace the
    // variable in the expression with a different variable
    virtual PTR(Expr) subst(std::string string, PTR(Expr) e) = 0;
    
    typedef enum {
        print_group_none,
        print_group_add,
        print_group_add_or_mult
    }   print_mode_t;
    
    virtual void print(std::ostream& out) = 0;
    virtual std::string to_string() = 0;
    void pretty_print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out) = 0;
    
    virtual void step_interp() = 0;
};

class NumExpr : public Expr {
public:
    PTR(Val) val;
    
    // constructor
    NumExpr(PTR(Val) val);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    // constructor
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    // constructor
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class VarExpr : public Expr {
public:
    std::string string;
    
    // constructor
    VarExpr(std::string string);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class LetExpr : public Expr {
public:
    std::string lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    // constructor
    LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class EqExpr : public Expr {
public:
    
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    // constructor
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) e);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class BoolExpr : public Expr {
public:
    
    PTR(Val) boolVal;
    
    // constructor
    BoolExpr(bool val);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class IfExpr : public Expr {
public:
    
    PTR(Expr) ifExpr;
    PTR(Expr) thenExpr;
    PTR(Expr) elseExpr;
    
    // constructor
    IfExpr(PTR(Expr) ifExpr, PTR(Expr) thenExpr, PTR(Expr) elseExpr);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class FunExpr : public Expr {
public:
    
    std::string formal_arg;
    PTR(Expr) body;
    
    // constructor
    FunExpr(std::string formal_arg, PTR(Expr) body);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

class CallExpr : public Expr {
public:
    
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    // constructor
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    
    bool equals(PTR(Expr) other_expr);
    PTR(Val) interp(PTR(Env) env);
    PTR(Expr) subst(std::string string, PTR(Expr) replacement);
    void print(std::ostream& output);
    std::string to_string();
    void pretty_print_at(print_mode_t mode, std::ostream& out);
    void step_interp();
    
};

