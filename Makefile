SRCS = main.cpp cmdline.cpp expr.cpp val.cpp parse.cpp ifExpr.cpp boolExpr.cpp callExpr.cpp funVal.cpp funExpr.cpp

msd-script: $(SRCS) cmdline.hpp expr.hpp catch.hpp val.hpp parse.hpp pointer.h
	c++ --std=c++14 -O2 -o msd-script $(SRCS)

.PHONY: test
test: msd-script
	./msd-script --test