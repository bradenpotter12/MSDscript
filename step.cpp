//
//  step.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/26/21.
//

#include "step.hpp"
#include "env.hpp"
#include "cont.hpp"
#include "expr.hpp"
#include "val.hpp"
#include "catch.hpp"

Step::mode_t Step::mode;
PTR(Expr) Step::expr;
PTR(Env) Step::env;
PTR(Val) Step::val;
PTR(Cont) Step::cont;

PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = NEW(EmptyEnv)();
    Step::val = nullptr;
    Step::cont = Cont::done;
    
    while (1) {
        if (Step::mode == Step::interp_mode) {
            Step::expr->step_interp();
        }
        else {
            if (Step::cont == Cont::done) {
                return Step::val;
            }
            else {
                Step::cont->step_continue();
            }
        }
    }
}

TEST_CASE( "interp_by_steps" ) {
    PTR(NumExpr) number1 = NEW(NumExpr)(NEW(NumVal)(1));
    PTR(NumExpr) number2 = NEW(NumExpr)(NEW(NumVal)(2));
    PTR(NumExpr) number3 = NEW(NumExpr)(NEW(NumVal)(3));
    PTR(IfExpr) ifTestTrue = NEW(IfExpr)(NEW(EqExpr)(number1, number1), number2, number3);
    PTR(IfExpr) ifTestFalse = NEW(IfExpr)(NEW(EqExpr)(number1, number2), number2, number3);
    
    CHECK(Step::interp_by_steps(ifTestTrue)->to_string() == "2");
    CHECK(Step::interp_by_steps(ifTestFalse)->to_string() == "3");
}
