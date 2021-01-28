//
//  main.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#include <iostream>
#include "cmdline.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "expr.hpp"




int main(int argc, const char * argv[]) {
    use_arguments(argc, argv);
    
    printf("Hello!\n");
    Num *c = new Num(3);
    
    return 0;
}
