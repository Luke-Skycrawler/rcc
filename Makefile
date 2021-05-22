# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
all: rcc.ypp rcc.l 
	bison -d rcc.ypp
	flex rcc.l
	g++ rcc.tab.cpp lex.yy.c -o rcc -lm
	# ./rcc
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.c
