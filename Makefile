SRCS = main.cpp cmdline.cpp expr.cpp val.cpp

msd-script: $(SRCS) cmdline.hpp expr.hpp catch.hpp val.hpp
	c++ --std=c++14 -O2 -o msd-script $(SRCS)

.PHONY: test
test: msd-script
	./msd-script --test