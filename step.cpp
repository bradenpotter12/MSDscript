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
    
    return 0;
}
