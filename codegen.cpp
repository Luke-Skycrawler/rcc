#include <vector>
#include "AST.hpp"
using namespace std;
using namespace llvm;
// Value *FunctionAST::codeGen()
// {
//     Function *func = topModule->getFunction(op);
//     if (!func)
//     {
//         vector<Type *> args(0, Type::getDoubleTy(context));
//         FunctionType *ft = FunctionType::get(Type::getDoubleTy(context), args, false);
//         func = Function::Create(ft, Function::ExternalLinkage, op, topModule);
//     }

//     BasicBlock *bb = BasicBlock::Create(context, "entry", func);
//     builder.SetInsertPoint(bb);
//     if (body)
//     {
//         if (auto ret = body->codeGen())
//         {
//             builder.CreateRet(ret);
//             verifyFunction(*func);

//             return ret;
//         }
//         if (func)
//             func->eraseFromParent();
//     }
//     return NULL;
// }
Value *Nstatement::codeGen()
{
    // auto tmp = expr->codeGen();
    // if (next)
    //     next->codeGen();
    // return tmp;
    return NULL;
}
// Value *BinaryExprAST::codeGen()
// {
//     Value *l = lhs->codeGen(), *r = rhs->codeGen();
//     switch (op[0])
//     {
//     case '+':
//         return builder.CreateFAdd(l, r, "add");
//     case '-':
//         return builder.CreateFSub(l, r, "sub");
//     case '*':
//         return builder.CreateFMul(l, r, "mult");
//     case '/':
//         return builder.CreateSDiv(l, r, "div");
//     case '|':
//         return builder.CreateOr(l, r, "or");
//     case '&':
//         return builder.CreateAnd(l, r, "and");
//     case '>':
//         return builder.CreateFCmpUGT(l, r, "");
//     case '<':
//         return builder.CreateFCmpULT(l, r, "cmp");
//     }
// }
// Value *CallExprAST::codeGen()
// {
//     Function *callee = topModule->getFunction(op);

//     vector<Value *> argv;
//     // for(auto it=args;it!=nullptr;it=it->next){
//     //     argv.push_back(it);
//     // }
//     return builder.CreateCall(callee, argv, "call");
// }
Value *Nconstant::codeGen()
{
    return ConstantFP::get(context, APFloat(value.double_value));
}
Value *Nidentifier::codeGen()
{
    Value *var=NULL;
    if(bindings.find(*name)!=bindings.end()){
        var = bindings[*name]->p.val;
    }
    return var;
}
Value *Ninitializer::codeGen()
{
    return NULL;
}
Value *NtypeSpecifier::codeGen()
{
    return NULL;
}
Value *NinitDeclarator::codeGen()
{
    return NULL;
}
Value *NdirectDeclarator::codeGen()
{
    return NULL;
}
Value *Ndeclarator::codeGen()
{
    return NULL;
}
Value *NdeclarationSpecifiers::codeGen()
{
    return NULL;
}
Value *Ndeclaration::codeGen()
{
    return NULL;
}
Value *NcompoundStatement::codeGen()
{
    if(declaration_list.size())
        for(auto it=declaration_list.begin();it!=declaration_list.end();it++)
            (*it)->codeGen();
    if (statement_list.size())
        for(auto it=statement_list.begin();it!=statement_list.end();it++)
            (*it)->codeGen();
    return NULL;
}