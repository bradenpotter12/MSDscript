SRCS = main.cpp cmdline.cpp

msd-script: $(SRCS) cmdline.hpp
	c++ --std=c++14 -O2 -o msd-script $(SRCS)