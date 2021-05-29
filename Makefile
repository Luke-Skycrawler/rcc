# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
LLVMFLAGS = -I/usr/lib/llvm-7/include  -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib/llvm-7/lib -lLLVM-7 -g
CC = g++
all: main.cpp rcc.tab.cpp lex.yy.c rccGlobal.hpp codegen.h codegen.cpp AST.hpp llvmGlobal.cpp
	${CC} ${LLVMFLAGS} main.cpp rcc.tab.cpp lex.yy.c codegen.cpp llvmGlobal.cpp -o rcc -lm
	./rcc test/easy.c
rcc.tab.cpp: rcc.ypp 
	bison -d rcc.ypp
lex.yy.c: rcc.l
	flex rcc.l
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.c location.hh position.hh stack.hh
