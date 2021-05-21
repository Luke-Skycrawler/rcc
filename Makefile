# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact
all: rcc.y rcc.l 
	bison -d rcc.y
	flex rcc.l
	gcc rcc.tab.c lex.yy.c -o rcc -lm
	# ./rcc
clean:
	rm rcc rcc.tab.c rcc.tab.h lex.yy.c
