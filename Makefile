# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
LLVMFLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all` -g
CC = g++
all: rcc.tab.cpp lex.yy.c codegen.h RccGlobal.hpp codegen.cpp AST.hpp llvmGlobal.cpp
	${CC} ${LLVMFLAGS} rcc.tab.cpp lex.yy.c codegen.cpp llvmGlobal.cpp -o rcc -lm
	gdb ./rcc
rcc.tab.cpp: rcc.ypp 
	bison -d rcc.ypp
lex.yy.c: rcc.l
	flex rcc.l
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.c location.hh position.hh stack.hh
