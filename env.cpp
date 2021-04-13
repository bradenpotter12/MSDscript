//
//  env.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 4/10/21.
//

#include "env.hpp"

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name)
        return val;
    else
        return rest->lookup(find_name);
}
