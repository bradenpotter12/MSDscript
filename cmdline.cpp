//
//  cmdline.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#include "cmdline.hpp"
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "expr.hpp"

void use_arguments(int argc, const char * argv[])
{
    bool testArgSeen = false;
    
    for (int i = 1; i < argc; i++)
    {
        std::string mainParameter = argv[i];
        
        if (mainParameter == "--help")
        {
            std::cout << "Arguments allowed: --help, --test\n";
            exit(0);
        }
        else if (mainParameter == "--test")
        {
            if (testArgSeen == false)
            {
                bool returnValue = Catch::Session().run();
                if (returnValue != 0) {
                    exit(1);
                }
                testArgSeen = true;
            }
            else
            {
                std::cerr << "--test may only be passed as a parameter once\n";
                exit(1);
            }
        }
        else if (mainParameter == "--interp") {
            while (1) {
            
                Expr * num = Expr::parse_expr(std::cin);
                std::cout << num->interp() << std::endl;
                
                Expr::skip_whitespace(std::cin);
                if (std::cin.eof()) {
                    break;
                }
            }
        }
        else
        {
            std::cerr << mainParameter + " is not a valid argument\n";
            exit(1);
        }
    }
}


