#ifndef _AST_H
#define _AST_H
#include "codegen.h"
extern llvm::LLVMContext context;
extern llvm::IRBuilder<> builder;
extern llvm::Module *topModule;
extern std::map<std::string,llvm::Value*> binding;
struct PrototypeAST
{
    std::string op;     // for drawing
    PrototypeAST(const std::string &op) : op(op){}
    virtual llvm::Function *codegen();
    bool isLeaf;
    void visit();
};
struct StmtAST: public PrototypeAST{
    StmtAST():op(';'){}
    llvm::Value *codegen() override;
    
    PrototypeAST *next;
};
struct BlockAST: public PrototypeAST{
    BlockAST():op('{'){}
    llvm::Value *codegen() override;

    PrototypeAST *next;
};
struct ForAST: public PrototypeAST{
    ForAST(StmtAST *s1,StmtAST *s2,StmtAST *s3,BlockAST *body):op('for'){}
    llvm::Value *codegen() override;

    StmtAST *cond[3];
    BlockAST *body;
};
struct IfAST: public PrototypeAST{
    IfAST(ExprAST *det,BlockAST *ybranch,BlockAST *nbranch=NULL):op('if'){}
    llvm::Value *codegen() override;
    
    ExprAST *det;
    BlockAST *ybranch,*nbranch;
};

struct DecAST: public PrototypeAST{
    DecAST(std::string &op);
    llvm::Value *codegen() override;
};

struct TypeAST: public PrototypeAST{
    TypeAST(std::string &op);
    llvm::Value *codegen() override;
};
struct FunctionAST: public PrototypeAST {
    FunctionAST(std::string &op,TypeAST *type,StmAST *body,DecAST *list);
    llvm::Value *codegen() override;

    StmtAST *body;
    DecAST *args;
    TypeAST *type;
};

struct ExprAST : public PrototypeAST{
    ExprAST(strd:string &op);
    llvm::Value *codegen() override;  
};
struct CallExprExt : public ExprAST{
    CallExprExt(strd:string &op);
    llvm::Value *codegen() override;  

    ExprAST *args;
};
struct UnaryExprAST: public ExprAST{
    UnaryExprAST()(std::string &op,ExprAST *exp);
    llvm::Value *codegen() override;
};
struct BinaryExprAST: public ExprAST{
    BinaryExprAST(std::string &op,ExprAST * lhs,ExprAST *rhs);
    llvm::Value *codegen() override;  
};
struct BinaryExprAST: public ExprAST{
    BinaryExprAST(std::string &op,ExprAST * lhs,ExprAST *rhs);
    llvm::Value *codegen() override;  
};
struct LiteralAST: public ExprAST{
    LiteralAST()(std::string &op);
    llvm::Value *codegen() override;

    float val;
};
struct VarAST: public ExprAST{
    VarAST()(std::string &op);
    llvm::Value *codegen() override;

    char baseType;
    PrototypeAST *AST type;
};
#endif