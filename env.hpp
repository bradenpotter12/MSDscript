//
//  env.hpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/10/21.
//

#ifndef env_hpp
#define env_hpp

#include <stdio.h>
#include <iostream>
#include "pointer.h"

class Val;

class Env : public std::enable_shared_from_this<Env> {
public:
    virtual PTR(Val) lookup(std::string find_name) = 0;
};

class EmptyEnv : public Env {
public:
    PTR(Val) lookup(std::string find_name);
};

class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    
    PTR(Val) lookup(std::string find_name);
};

#endif /* env_hpp */
