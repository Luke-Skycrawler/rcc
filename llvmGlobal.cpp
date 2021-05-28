#include "ast.h"
using namespace llvm;

LLVMContext context;
llvm::IRBuilder<> builder(context);
Module *topModule;
std::map<std::string,Value *> binding;
PrototypeAST *root;