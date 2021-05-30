#include "AST.hpp"
#include <vector>
using namespace std;
using namespace llvm;
int indent=0;
static const int volume=255;
vector<Function *> funcStack;
Value *FunctionAST::codegen()
{
    Function *func = topModule->getFunction(op);
    if (!func)
    {
        vector<Type *> args(0, Type::getDoubleTy(context));
        FunctionType *ft = FunctionType::get(Type::getDoubleTy(context), args, false);
        func = Function::Create(ft, Function::ExternalLinkage, "main", topModule);
        funcStack.push_back(func);
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
    funcStack.pop_back();
    return NULL;
}
Value *DecAST::codegen(){
    auto t=funcStack[funcStack.size()-1];
    // llvm::IRBuilder<> builder(&t->getEntryBlock(), t->getEntryBlock().begin());
    auto allocation = builder.CreateAlloca(Type::getDoubleTy(context), NULL, op);
    builder.CreateStore(builder.getInt32(0), allocation);
    if(next)next->codegen();
    bindings[op]=allocation;
    return allocation;
}
Value *IfAST::codegen(){
    return NULL;
}
Value *ForAST::codegen(){
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
    default :
        return NULL;
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
    return bindings[op];
    // return var;
}
Value *VarAST::allocate(Constant *initial){
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
    auto ret=content?content->codegen():NULL;
    if(next)next->codegen();
    return ret;
}
/*llvm::Function* createPrintf()
{
    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(TheBuilder.getInt8PtrTy());
    auto printf_type = llvm::FunctionType::get(TheBuilder.getInt32Ty(), llvm::makeArrayRef(arg_types), true);
    auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), topModule.get());
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}
llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::StringRef VarName, llvm::Type* type)
{
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
}
        auto alloc = CreateEntryBlockAlloca(generator.getCurFunction(), name, this->type->toLLVMType());
  return TmpB.CreateAlloca(type, nullptr, VarName);
        return TheBuilder.CreateStore(this->value->codeGen(generator), alloc);


llvm::Function* createScanf()
{
    auto scanf_type = llvm::FunctionType::get(TheBuilder.getInt32Ty(), true);
    auto func = llvm::Function::Create(scanf_type, llvm::Function::ExternalLinkage, llvm::Twine("scanf"), topModule.get());
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}*/
