#include "AST.hpp"
#include <vector>
using namespace llvm;
LLVMContext context;
IRBuilder<> builder(context);
Module *topModule;
std::map<std::string,AllocaInst *> bindings;
std::vector<PrototypeAST *>root;