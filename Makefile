# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
LLVMFLAGS = `/usr/local/Cellar/llvm@4/4.0.1/bin/llvm-config --cxxflags --ldflags --system-libs --libs all`
all: rcc.ypp rcc.l 
	bison -d rcc.ypp
	flex rcc.l
	g++ ${LLVMFLAGS} rcc.tab.cpp lex.yy.c -o rcc -lm
	# ./rcc
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.c
