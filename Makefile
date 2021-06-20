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
marco_test: lex.marco.cpp marco.tab.cpp marco.cpp marco.hpp
	${CXX} -D MARCO_DEBUG -g -o $@ lex.marco.cpp marco.tab.cpp marco.cpp
	./marco_test test/define.c
	cat .LONG_AND_AWARD_NAME.c
# marco.o: lex.marco.cpp marco.tab.cpp marco.cpp
# 	${CXX} -g -o marco.o lex.marco.cpp marco.tab.cpp marco.cpp
# 	@echo "CXX $< => $@"
marco.tab.cpp: marco.ypp 
	${BISON} -d marco.ypp
lex.marco.cpp: marco.l
	flex -Pmarco marco.l 
	mv lex.marco.c lex.marco.cpp
	# flex marco.l 
	# mv lex.yy.c lex.marco.cpp
	
%.o: %.cpp AST.hpp
	${CXX} -c ${DEFINE} ${CXXFLAGS} -g -o $@ $<
	@echo "CXX $< => $@"
rcc: rcc.tab.o lex.yy.o CodeGen.o AST.o main.o lex.marco.o marco.tab.o marco.o
	${CXX} -o $@ rcc.tab.o lex.yy.o CodeGen.o AST.o main.o lex.marco.o marco.tab.o marco.o ${LIBS} ${LDFLAGS}
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