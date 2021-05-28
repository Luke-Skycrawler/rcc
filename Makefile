all: rcc.ypp rcc.l 
	bison -d rcc.ypp
	flex rcc.l
	mv lex.yy.c lex.yy.cpp
	g++ rcc.tab.cpp lex.yy.cpp -o rcc -lm
	# ./rcc
clean:
	rm rcc rcc.tab.cpp rcc.tab.hpp lex.yy.cpp location.hh position.hh stack.hh

