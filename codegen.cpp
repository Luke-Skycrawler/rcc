#include "ast.h"
#include <vector>
using namespace std;
using namespace llvm;
Value *FunctionAST::codegen()
{
    Function *func = topModule->getFunction(op);
    if (!func)
    {
        vector<Type *> args(0, Type::getDoubleTy(context));
        FunctionType *ft = FunctionType::get(Type::getDoubleTy(context), args, false);
        func = Function::Create(ft, Function::ExternalLinkage, op, topModule);
    }

    BasicBlock *bb = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(bb);
    if (body)
    {
        if (auto ret = body->codegen())
        {
            builder.CreateRet(ret);
            verifyFunction(*func);

            return ret;
        }
        if (func)
            func->eraseFromParent();
    }
    return NULL;
}
Value *StmtAST::codegen()
{
    auto tmp = expr->codegen();
    if (next)
        next->codegen();
    return tmp;
}
Value *BinaryExprAST::codegen()
{
    Value *l = lhs->codegen(), *r = rhs->codegen();
    switch (op[0])
    {
    case '+':
        return builder.CreateFAdd(l, r, "add");
    case '-':
        return builder.CreateFSub(l, r, "sub");
    case '*':
        return builder.CreateFMul(l, r, "mult");
    case '/':
        return builder.CreateSDiv(l, r, "div");
    case '|':
        return builder.CreateOr(l, r, "or");
    case '&':
        return builder.CreateAnd(l, r, "and");
    case '>':
        return builder.CreateFCmpUGT(l, r, "");
    case '<':
        return builder.CreateFCmpULT(l, r, "cmp");
    }
}
Value *CallExprAST::codegen()
{
    Function *callee = topModule->getFunction(op);

    vector<Value *> argv;
    // for(auto it=args;it!=nullptr;it=it->next){
    //     argv.push_back(it);
    // }
    return builder.CreateCall(callee, argv, "call");
}
Value *LiteralAST::codegen()
{
    return ConstantFP::get(context, APFloat(val));
}
Value *VarAST::codegen()
{
    Value *var = bindings[op];
    return var;
}
Value *UnaryExprAST::codegen()
{
    return NULL;
}
Value *CommaExprAST::codegen()
{
    return NULL;
}
Value *BlockAST::codegen()
{
    if (content)
        return content->codegen();
    else
        return NULL;
}