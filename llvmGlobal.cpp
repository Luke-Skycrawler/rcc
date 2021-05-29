#include "ast.h"
using namespace llvm;

LLVMContext context;
IRBuilder<> builder(context);
Module *topModule;
std::map<std::string,BindingEntry*> bindings;
PrototypeAST *root;