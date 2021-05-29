#ifndef _CODEGEN_H
#define _CODEGEN_H
#include <map>
#include <string>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
extern llvm::LLVMContext context;
extern llvm::IRBuilder<> builder;
extern llvm::Module *topModule;
extern std::map<std::string, llvm::Value *> bindings;
#endif