SRCS = main.cpp cmdline.cpp expr.cpp

msd-script: $(SRCS) cmdline.hpp expr.hpp catch.hpp
	c++ --std=c++14 -O2 -o msd-script $(SRCS)