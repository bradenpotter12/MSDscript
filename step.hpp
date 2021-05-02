//
//  step.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/26/21.
//

#ifndef step_hpp
#define step_hpp

#include <stdio.h>
#include "pointer.h"

class Expr;
class Cont;
class Env;
class Val;

class Step {
public:
    
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;

    static mode_t mode;     /* chooses mode */
    
    static PTR(Expr) expr;  /* for interp_mode */
    static PTR(Env) env;    /* for interp mode */
    
    static PTR(Val) val;    /* for continue_mode */
    
    static PTR(Cont) cont;  /* all modes */
    
    PTR(Val) interp_by_steps(PTR(Expr) e);
};
#endif /* step_hpp */
