#include "codegen.h"
using namespace llvm

LLVMContext context;
llvm::IRBuilder<> Builder(context);
Module *topModule;
std::map<std::string,Value *> binding;