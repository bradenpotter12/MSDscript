//
//  run.cpp
//  test
//
//  Created by Braden Potter on 2/4/21.
//

extern "C" {
#include "run.hpp"
};

#define CATCH_CONFIG_RUNNER
#include "../catch.hpp"

bool run_tests() {
    const char *argv[] = { "arith" };
    return (Catch::Session().run(1, argv) == 0);
}
