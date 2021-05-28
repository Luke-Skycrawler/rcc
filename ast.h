#ifndef _AST_H
#define _AST_H
#include "codegen.h"
struct ExprAST;
struct PrototypeAST
{
    std::string op; // for drawing
    PrototypeAST(const std::string &op) : op(op) {}
    /*virtual*/ llvm::Function *codegen();
    bool isLeaf;
    /*virtual*/ void visit();
};
struct StmtAST : public PrototypeAST
{
    StmtAST() : PrototypeAST(";") {}
    llvm::Value *codegen() /*override*/;

    ExprAST *expr;
    PrototypeAST *next;
};
struct BlockAST : public PrototypeAST
{
    BlockAST() : PrototypeAST("{") {}
    llvm::Value *codegen() /*override*/;

    PrototypeAST *next,*content;
};
struct ForAST : public BlockAST
{
    ForAST(StmtAST *s1, StmtAST *s2, StmtAST *s3, BlockAST *body);
    llvm::Value *codegen() /*override*/;

    StmtAST *header[3];
    BlockAST *body;
};
struct IfAST : public PrototypeAST
{
    IfAST(ExprAST *det, BlockAST *ybranch, BlockAST *nbranch = NULL) : PrototypeAST("if") {}
    llvm::Value *codegen() /*override*/;

    ExprAST *det;
    BlockAST *ybranch, *nbranch;
};

struct DecAST : public PrototypeAST
{
    DecAST(std::string &op);
    llvm::Value *codegen() /*override*/;
};

struct TypeAST : public PrototypeAST
{
    TypeAST(std::string &op);
    llvm::Value *codegen() /*override*/;
};
struct FunctionAST : public PrototypeAST
{
    FunctionAST(std::string &op, TypeAST *type, StmtAST *body, DecAST *list);
    llvm::Function *codegen() /*override*/;

    StmtAST *body;
    DecAST *args;
    TypeAST *type;
};

struct ExprAST : public PrototypeAST
{
    ExprAST(std::string &op);
    virtual llvm::Value *codegen();
};
struct CommaExprAST : public ExprAST{
    CommaExprAST(std::string &op);
    llvm::Value *codegen() override;

    ExprAST *next;
};
struct CallExprAST : public ExprAST
{
    CallExprAST(std::string &op);
    llvm::Value *codegen() override;

    ExprAST *args;
};
struct UnaryExprAST : public ExprAST
{
    UnaryExprAST(std::string &op, ExprAST *exp);
    llvm::Value *codegen() override;
};
struct BinaryExprAST : public ExprAST
{
    BinaryExprAST(std::string &op, ExprAST *lhs, ExprAST *rhs);
    llvm::Value *codegen() override;

    ExprAST *lhs,*rhs;
};
struct LiteralAST : public ExprAST
{
    LiteralAST(std::string &op);
    llvm::Value *codegen() override;

    float val;
};
struct VarAST : public ExprAST
{
    VarAST(std::string &op);
    llvm::Value *codegen() override;

    char baseType;
    PrototypeAST *type;
};
#endif