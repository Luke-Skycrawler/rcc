# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
BISON ?= bison
LLVMFLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all` -g
CXX = g++
all: rcc.tab.cpp lex.yy.cpp codegen.h RccGlobal.hpp codegen.cpp AST.hpp llvmGlobal.cpp
	${CXX} ${LLVMFLAGS} rcc.tab.cpp lex.yy.cpp codegen.cpp llvmGlobal.cpp -o rcc -lm -Wno-deprecated-register
	@echo "CXX & LINK => rcc"
rcc.tab.cpp: rcc.ypp 
	${BISON} -d rcc.ypp
	@echo "BISON => $@"
lex.yy.cpp: rcc.l
	flex rcc.l
	mv lex.yy.c lex.yy.cpp
	@echo "FLEX => $@"
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.cpp location.hh position.hh stack.hh
