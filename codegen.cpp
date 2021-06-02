#include "AST.hpp"
#include <vector>
using namespace std;
using namespace llvm;
int indent = 0;
static const int volume = 255;
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
Value *DecAST::codegen()
{
    auto t = funcStack[funcStack.size() - 1];
    // llvm::IRBuilder<> builder(&t->getEntryBlock(), t->getEntryBlock().begin());
    auto allocation = builder.CreateAlloca(Type::getDoubleTy(context), NULL, op);
    switch(baseType){
        case 'd':
            allocation = builder.CreateAlloca(Type::getDoubleTy(context),NULL,op);
            if(init)builder.CreateStore(init->codegen(),allocation);
            break;
        case 'i':
            allocation = builder.CreateAlloca(Type::getInt32Ty(context),NULL,op);
            builder.CreateStore(init?init->codegen():builder.getInt32(0),allocation);
            break;
        case 'c':
            allocation = builder.CreateAlloca(Type::getInt8Ty(context),NULL,op);
            builder.CreateStore(init?init->codegen():builder.getInt8(0),allocation);
            break;
    }
    // builder.CreateStore(builder.getInt64(0), allocation);
    bindings[op] = allocation;
    if (next)
        next->codegen();
    return allocation; // some arbitary pointer other than NULL
}
Value *IfAST::codegen()
{
    return NULL;
}
Value *ForAST::codegen()
{
    return NULL;
}
Value *StmtAST::codegen()
{
    auto tmp = expr->codegen();
    if (next)
        next->codegen();
    return tmp;
}
inline char typeCheck(ExprAST *lhs,ExprAST *rhs,const string &op){
    return 'd';
}
inline Value *createOpNode(Value *l,Value *r,char op){
    switch (op)
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
    default:
        return NULL;
    }
}
Value *BinaryExprAST::codegen()
{
    Value *l = lhs->codegen(), *r = rhs->codegen();
    char type=typeCheck(lhs,rhs,op);
    return createOpNode(l,r,op[0]);
}
Value *AssignAST::codegen(){
    Value *r=rhs->codegen(),*l=NULL,*result=NULL;
    auto storeAddr=bindings[lhs->op];
    if(op[0]!='='){
        l=lhs->codegen();
        result=createOpNode(l,r,op[0]);
    }
    else result=r;
    builder.CreateStore(result,storeAddr);
    return result;
}
Value *CallExprAST::codegen()
{
    if(op=="printf")CreatePrintf();
    Function *callee = topModule->getFunction(op);
    vector<Value *> argv;
    for(auto it=args;it!=NULL;it=it->next){
        argv.push_back(it->codegen());
    }
    return builder.CreateCall(callee, argv, "call");
    
}
Value *LiteralAST::codegen()
{
    if(op[0]!='"')return ConstantFP::get(context, APFloat(val));
    auto str= ConstantDataArray::getString(context,op);
	auto addr= builder.CreateAlloca(str->getType(), ConstantExpr::getSizeOf(str->getType()),"str_addr");
    addr->setAlignment (1);		   	
	Value* p = builder.CreateGEP(addr, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
	builder.CreateStore(str, p)->setAlignment(1);
    return p;
    // return builder.CreateInBoundsGEP( str->getType(), addr, index_vector, "tmpstr");    
}
Value *VarAST::codegen()
{
    return builder.CreateLoad(bindings[op]);
    // return bindings[op];
}
Value *UnaryExprAST::codegen()
{
    return NULL;
}
Value *BlockAST::codegen()
{
    auto ret = content ? content->codegen() : NULL;
    if (next)
        next->codegen();
    return ret;
}
llvm::Value *DerivedTypeAST::codegen(){
    return NULL;
}
// Value *FunctionDecAST::codegen(){
//     std::vector<llvm::Type *> arg_types;
//     arg_types.push_back(builder.getInt8PtrTy());
//     auto printf_type = llvm::FunctionType::get(builder.getInt32Ty(), llvm::makeArrayRef(arg_types), true);
//     auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("op"), topModule);
//     func->setCallingConv(llvm::CallingConv::C);
//     return func;
// }

// llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::StringRef VarName, llvm::Type* type)
// {
//   llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
// }
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
