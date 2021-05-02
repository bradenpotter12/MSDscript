SRCS = main.cpp cmdline.cpp expr.cpp val.cpp parse.cpp ifExpr.cpp boolExpr.cpp callExpr.cpp funVal.cpp funExpr.cpp env.cpp eqExpr.cpp letExpr.cpp step.cpp cont.cpp

msd-script: $(SRCS) cmdline.hpp expr.hpp catch.hpp val.hpp parse.hpp pointer.h env.hpp step.hpp cont.hpp
	c++ --std=c++14 -fsanitize=undefined -fno-sanitize-recover=undefined -O2 -o msd-script $(SRCS)

.PHONY: test
test: msd-script
	./msd-script --test