# win_flex.exe --bison-bridge rcc.l
# --header-file=lex.yy.h --winconmpact

BISON ?= bison
LLVM_CONFIG ?= llvm-config
NO_WARNING =  -Wno-return-type \
	-Wno-c++11-compat-deprecated-writable-strings \
	-Wno-deprecated-register \
	-Wno-switch \

CXXFLAGS = `${LLVM_CONFIG} --cxxflags` ${NO_WARNING} -fpermissive -fexceptions
LDFLAGS = `${LLVM_CONFIG} --ldflags`
LIBS = `$(LLVM_CONFIG) --libs --system-libs`

CXX = g++
all: rcc
macro_test: lex.macro.cpp macro.ypp macro.cpp macro.hpp
	@echo "BISON => macro.tab.cpp"
	${BISON} -d macro.ypp
	@echo "CXX lex.macro.cpp macro.tab.cpp macro.cpp => $@"
	${CXX} -D MACRO_DEBUG -D NO_LOGGING -g -o $@ lex.macro.cpp macro.tab.cpp macro.cpp
	# ./macro_test test/define.c
	# cat .LONG_AND_AWARD_NAME.c
lex.macro.cpp: macro.l
	flex -Pmacro macro.l 
	mv lex.macro.c lex.macro.cpp
	# flex macro.l 
	# mv lex.yy.c lex.macro.cpp
	
%.o: %.cpp AST.hpp
	${CXX} -c ${DEFINE} ${CXXFLAGS} -g -o $@ $<
	@echo "CXX $< => $@"
# rcc: rcc.tab.o lex.yy.o CodeGen.o AST.o main.o lex.macro.cpp macro.ypp macro.cpp
# 	mkdir tmp
# 	cp macro.ypp lex.macro.cpp macro.cpp tmp
# 	cd tmp 
# 	${BISON} -d macro.ypp
# 	${CXX} -c -g -o macro.tab.o macro.tab.cpp
# 	${CXX} -c -g -o lex.macro.o lex.macro.cpp
# 	${CXX} -c -g -o macro.o macro.cpp
# 	cp macro.tab.o lex.macro.o macro.o ../
# 	cd ..
# 	rm -r tmp
# 	@echo "CXX $< => $@"
# 	${CXX} -o $@ rcc.tab.o lex.yy.o CodeGen.o AST.o main.o lex.macro.o macro.tab.o macro.o ${LIBS} ${LDFLAGS}
# 	@echo "LINK * => rcc"
rcc: rcc.tab.o lex.yy.o CodeGen.o AST.o main.o macro_test
	${CXX} -o $@ rcc.tab.o lex.yy.o CodeGen.o AST.o main.o ${LIBS} ${LDFLAGS}
	@echo "LINK * => rcc"
rcc.tab.cpp: rcc.ypp 
	${BISON} -d rcc.ypp
	@echo "BISON => $@"
lex.yy.cpp: rcc.l
	flex rcc.l
	mv lex.yy.c lex.yy.cpp
	@echo "FLEX => $@"
.PHONY: clean
clean:
	-rm -f rcc
	-rm -f *.o
	-rm -f rcc.tab.cpp
	-rm -f rcc.tab.hpp
	-rm -f lex.*
	-rm -f lex.yy.cpp
	-rm -f location.hh
	-rm -f position.hh
	-rm -f stack.hh
	-rm -f macro_test
	-rm -rf macro_test.dSYM
	-rm -f .LONG_AND_AWARD_NAME.c