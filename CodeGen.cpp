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
    // If one is double, the op should also be a double op
    bool double_flag = l->getType()->isDoubleTy() || r->getType()->isDoubleTy();
    std::cout << "LHS is double?: " << l->getType()->isDoubleTy() << std::endl;
    std::cout << "RHS is double?: " << r->getType()->isDoubleTy() << std::endl;
    // If only one is double but the other is int, convert the int -> double...
    if(double_flag && !l->getType()->isDoubleTy()) l = builder.CreateSIToFP(l, Type::getDoubleTy(context));
    if(double_flag && !r->getType()->isDoubleTy()) r = builder.CreateSIToFP(r, Type::getDoubleTy(context));

    if(!double_flag){
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
    return NULL;
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
    if(type == "char") return builder.getInt8(value.char_value);
    else if(type == "int")
    {
        return builder.getInt32(value.int_value);
    }
    else if(type == "double") return ConstantFP::get(context, APFloat(value.double_value));

    else if(type =="string_literal")
    {
        string op(value.string_literal_value);
        auto str= ConstantDataArray::getString(context, op);
        auto addr= builder.CreateAlloca(str->getType(), ConstantExpr::getSizeOf(str->getType()), "str_addr");
        #ifdef MACOS
        addr->setAlignment(Align(1));
        #else
        addr->setAlignment(1);
        #endif
        llvm::Value* p = builder.CreateGEP(addr, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
        #ifdef MACOS
        builder.CreateStore(str, p)->setAlignment(Align(1));
        #else
        builder.CreateStore(str, p)->setAlignment(1);
        #endif
        return p;
    }
    return NULL;
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
        ret = allocation;
        builder.CreateStore(builder.getInt64(0), allocation);
        bindings[op] = allocation;
        // if (next)
        //     next->codeGen();
    } 
    return (Value*)ret; // some arbitary pointer other than NULL
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
Value *NifStatement::codeGen()
{
    Value* cond_val = cond_expr->codeGen();
    printf("Hola2\n");
    if(!cond_val)
    {
        printf("Error: conditional expression is not valid!\n");
        return NULL;
    }
    
    // If the expr is not a double, convert it to a double
    if(!cond_val->getType()->isDoubleTy()) cond_val = builder.CreateSIToFP(cond_val, Type::getDoubleTy(context));
    
    // cond_val = builder.CreateICmpNE(cond_val, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true), "ifcond");
    cond_val = builder.CreateFCmpONE(cond_val, ConstantFP::get(context, APFloat(0.0)), "ifcond");
    
    Function *the_function = builder.GetInsertBlock()->getParent();
    
    BasicBlock* then_bb = BasicBlock::Create(context, "then", the_function);
    BasicBlock* else_bb = BasicBlock::Create(context, "else");
    BasicBlock* merge_bb = BasicBlock::Create(context, "ifcont");

    Value* ret = builder.CreateCondBr(cond_val, then_bb, else_bb);

    builder.SetInsertPoint(then_bb); // set insert point to `then_bb`
    Value *then_val = if_statement->codeGen(); // recursively codeGen()
    if(!then_val)
    {
        printf("Error: if statement is not valid!\n");
        return NULL;
    }
    builder.CreateBr(merge_bb); // unconditional branch to the merge point
    then_bb = builder.GetInsertBlock(); // update `then_bb`

    the_function->getBasicBlockList().push_back(else_bb); // push `else_bb` into the BB list
    builder.SetInsertPoint(else_bb); // set insert point to `else_bb`
    Value* else_val = NULL;
    if(else_statement) else_val = else_statement->codeGen(); // recursively codeGen()
    builder.CreateBr(merge_bb); // unconditional branch to the merge point
    else_bb = builder.GetInsertBlock(); // update `else_bb`

    the_function->getBasicBlockList().push_back(merge_bb); // push `merge_bb` into the BB list
    builder.SetInsertPoint(merge_bb); // set insert point to `merge_bb`

    return ret;
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
