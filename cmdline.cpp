//
//  cmdline.cpp
//  msd-script-cs6015
//
//  Created by Braden Potter on 1/19/21.
//

#include "cmdline.hpp"
#include <iostream>

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
                std::cout << "Tests passed\n";
                testArgSeen = true;
            }
            else
            {
                std::cerr << "--test may only be passed as a parameter once\n";
                exit(1);
            }
        }
        else
        {
            std::cerr << mainParameter + " is not a valid argument\n";
            exit(1);
        }
    }
}
