# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
LLVMFLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all`
CC = g++
all: rcc.ypp rcc.l global.hpp codegen.h codegen.cpp ast.h llvmGlobal.cpp
	bison -d rcc.ypp
	flex rcc.l
	${CC} ${LLVMFLAGS} rcc.tab.cpp lex.yy.c codegen.cpp llvmGlobal.cpp -o rcc -lm
	# ./rcc
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.c location.hh position.hh stack.hh
