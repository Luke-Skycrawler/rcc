#include <vector>
#include "AST.hpp"
using namespace std;
using namespace llvm;
Value *Nstatement::codeGen()
{
    // auto tmp = expr->codeGen();
    // if (next)
    //     next->codeGen();
    // return tmp;
    return NULL;
}
Value *NbinaryExpr::codeGen()
{
    Value *l = lhs->codeGen(), *r = rhs->codeGen();
    if(type=="int"){
        switch(op[0]){
            case '+':return builder.CreateAdd(l,r);
            case '-':return builder.CreateSub(l,r);
            case '*':return builder.CreateMul(l,r);
            case '/':return builder.CreateSDiv(l,r);
            case '%':return builder.CreateSRem(l,r);
            case '&':
                if(op.size()==1)
                    return builder.CreateAnd(l,r);
                else return builder.CreateAnd(l,r);
            case '|':return builder.CreateOr(l,r);
            case '^':return builder.CreateXor(l,r);
            case '<':
                if(op.size()==1)return builder.CreateICmpSLT(l,r);
                else if(op[1]=='=')return builder.CreateICmpSLE(l,r);
                else if(op[1]=='<')return builder.CreateShl(l,r);
            case '>':
                if(op.size()==1) return builder.CreateICmpSGT(l,r);
                else if (op[1]=='=')return builder.CreateICmpSGE(l,r);
                else if(op[1]=='>') return builder.CreateAShr(l,r);
            case '=':return builder.CreateICmpEQ(l,r);
            case '!':return builder.CreateICmpNE(l,r);
        }
    }
    else {
        switch (op[0])
        {
        case '+':
            return builder.CreateFAdd(l, r, "add");
        case '-':
            return builder.CreateFSub(l, r, "sub");
        case '*':
            return builder.CreateFMul(l, r, "mult");
        case '/':
            return builder.CreateFDiv(l, r, "div");
        case '>':
            if(op.size()==1)return builder.CreateFCmpUGT(l, r, "");
            else return builder.CreateFCmpUGE(l, r, "");
        case '<':
            if(op.size()==1)return builder.CreateFCmpULT(l, r, "cmp");
            return builder.CreateFCmpULE(l, r, "cmp");
        default:
            return NULL;
        }
    }
}
// Value *CallExprAST::codeGen()
// {
//     Function *callee = topModule->getFunction(op);

//     vector<Value *> argv;
//     // for(auto it=args;it!=nullptr;it=it->next){
//     //     argv.push_back(it);
//     // }
//     return builder.CreateCall(callee, argv, "call");
// }
llvm::Value *Nconstant::codeGen()
{

    if(type!="string_literal")return ConstantFP::get(context, APFloat(value.double_value));
    string op(value.string_literal_value);
    auto str= ConstantDataArray::getString(context,op);
	auto addr= builder.CreateAlloca(str->getType(), ConstantExpr::getSizeOf(str->getType()),"str_addr");
    addr->setAlignment (1);		   	
	llvm::Value* p = builder.CreateGEP(addr, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
	builder.CreateStore(str, p)->setAlignment(1);
    return p;
}
Value *Nidentifier::codeGen()
{
    return builder.CreateLoad(bindings[name]);
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
// Value *Ndeclarator::codeGen()
// {
//     return NULL;
// }
Value *NdeclarationSpecifiers::codeGen()
{
    return NULL;
}
Value *Ndeclaration::codeGen()
{
    // Node *next=NULL;
    void * ret;
    for(auto it:init_declarator_list){
        auto op=dynamic_cast<NdirectDeclarator*>(it)->identifier->name;
        auto allocation = builder.CreateAlloca(Type::getDoubleTy(context), NULL, op);
        ret=allocation;
        builder.CreateStore(builder.getInt64(0), allocation);
        bindings[op] = allocation;
        // if (next)
        //     next->codeGen();
    } 
    return ret; // some arbitary pointer other than NULL
}
Value *NcompoundStatement::codeGen()
{
    Value *tmp;
    if(declaration_list.size())
        for(auto it=declaration_list.begin();it!=declaration_list.end();it++)
            tmp=(*it)->codeGen();
    if (statement_list.size())
        for(auto it=statement_list.begin();it!=statement_list.end();it++)
            tmp=(*it)->codeGen();
    return tmp;
}
Value *NfunctionDefinition::codeGen()
{
    string op="";
    Node *body=compound_statement;

    Function *func = topModule->getFunction(op);
    if (!func)
    {
        vector<Type *> args(0, Type::getDoubleTy(context));
        FunctionType *ft = FunctionType::get(Type::getDoubleTy(context), args, false);
        func = Function::Create(ft, Function::ExternalLinkage, "main", topModule);
        // funcStack.push_back(func);
    }

    BasicBlock *bb = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(bb);
    if (body)
    {
        if (auto ret = body->codeGen())
        {
            builder.CreateRet(ret);
            verifyFunction(*func);

            return ret;
        }
        // if (func)
        //     func->eraseFromParent();
    }
    // funcStack.pop_back();
    return NULL;
}
Value *Nprogram::codeGen()
{
    for(auto it:external_declaration_list){
        it->codeGen();
    }
    return NULL;
}
Value *NexprStatement::codeGen()
{
    return expr->codeGen();
}
Value *Nexpr::codeGen()
{
    Value *tmp;
    for(auto it:expr_list){
        tmp=it->codeGen();
    }
    return tmp;
}
Value *NassignExpr::codeGen()
{
    return NULL;
}
Value *NcondExpr::codeGen()
{
    return NULL;
}
Value *NcastExpr::codeGen()
{
    return NULL;
}
Value *NunaryExpr::codeGen()
{
    return NULL;
}
Value *NpostfixExpr::codeGen()
{
    if(postfix_type==PARENTHESES){
        string op=dynamic_cast<Nidentifier*>(postfix_expr)->name;
        if(op=="printf")CreatePrintf();
        Function *callee = topModule->getFunction(op);
        vector<Value *> argv;
        for(auto it:argument_expr_list){
            argv.push_back(it->codeGen());
        }
        return builder.CreateCall(callee, argv, "call");
    }
    return NULL;
}
Value *NparameterDeclaration::codeGen()
{
    return NULL;
}
llvm::Function *CreatePrintf()
{
    std::vector<llvm::Type *> arg_types;
    arg_types.push_back(builder.getInt8PtrTy());
    auto printf_type = llvm::FunctionType::get(builder.getInt32Ty(), llvm::makeArrayRef(arg_types), true);
    auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), topModule);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}
llvm::Function *CreateScanf()
{
    auto scanf_type = llvm::FunctionType::get(builder.getInt32Ty(), true);
    auto func = llvm::Function::Create(scanf_type, llvm::Function::ExternalLinkage, llvm::Twine("scanf"), topModule);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}
