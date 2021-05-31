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
%.o: %.cpp
	${CXX} -c ${CXXFLAGS} -g -o $@ $<
	@echo "CXX $< => $@"
rcc: rcc.tab.o lex.yy.o CodeGen.o LLVMGlobal.o AST.o
	${CXX} -o $@ rcc.tab.o lex.yy.o CodeGen.o LLVMGlobal.o AST.o ${LIBS} ${LDFLAGS}
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
	-rm -f lex.yy.c
	-rm -f lex.yy.cpp
	-rm -f location.hh
	-rm -f position.hh
	-rm -f stack.hh