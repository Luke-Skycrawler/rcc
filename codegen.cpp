#include "ast.h"
#include <vector>
using namespace std;
using namespace llvm;
Function *FunctionAST::codegen(){
    Function *func=topModule->getFunction(op);
    basicBlock *bb=BacsicBlock::Create(context,"entry",func);
    builder.SetInsertPoint(bb);

    if(auto ret=boty->codegen()){
        builder.CreateRet(ret);
        verifyFunction(*func);

        return func;
    }
    func->eraseFromParent();
    return nullptr;
}

Value *BinaryExprAST::codegen(){
    Value *l=lhs->codegen(),r=rhs->codegen();
    switch(op){
        case '+':return builder.CreateFAdd(l,r,"add");
        case '-':return builder.CreateFSub(l,r,"sub");
        case '*':return builder.CreateFMul(l,r,"mult");
        case '<':return builder.CreateFCmpULT(l,r,"cmp");
        // case '/':return builder.CreateFAdd(l,r,"add");
        // case '/':return builder.CreateFAdd(l,r,"add");
        // case '/':return builder.CreateFAdd(l,r,"add");
        // case '/':return builder.CreateFAdd(l,r,"add");
    }
}
Value *CallExprAST::codegen(){
    Function callee=topModule->getFunction(op);
    
    vector<Value *> argv;
    for(auto *it=args;it!=nullptr;it=it->next){
        argv.push_back(it);
    }
    return builder.CreateCall(callee,argv,"call")
}
Value *LiteralExpr::codegen(){
    return ConstantFP::get(context,APFloat(val));
}
Value *VarAST::codegen(){
    Value *var = binding[op];
    return var;
}