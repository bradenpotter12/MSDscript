//
//  cont.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/26/21.
//

#include "cont.hpp"
#include "step.hpp"
#include "expr.hpp"

PTR(Cont) Cont::done = NEW(DoneCont)();

void IfBranchCont::step_continue()
{
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    
    if (Step::expr->equals(NEW(BoolExpr)(true))) {
        Step::expr = then_part;
    }
    else {
        Step::expr = else_part;
    }
    Step::env = env;
    Step::cont = rest;
}

void LetBodyCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(var, Step::val, env);
    
    Step::cont = rest;
}

