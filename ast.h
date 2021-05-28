#ifndef _AST_H
#define _AST_H
#include "codegen.h"
struct ExprAST;
struct PrototypeAST
{
    std::string op; // for drawing
    PrototypeAST(const std::string &op) : op(op) {printf("\n");}
    /*virtual*/ llvm::Function *codegen();
    bool isLeaf;
    virtual void visit() = 0;
};
struct StmtAST : public PrototypeAST
{
    StmtAST(ExprAST *expr = nullptr, PrototypeAST *next = nullptr) : PrototypeAST(";"), expr(expr), next(next) {printf("%s","StmtAST\n");}
    llvm::Value *codegen() /*override*/;

    ExprAST *expr;
    PrototypeAST *next;
    void visit() override {}
};
struct BlockAST : public PrototypeAST
{
    BlockAST(PrototypeAST *content = nullptr, PrototypeAST *next = nullptr)
        : PrototypeAST("{"), content(content), next(next) {printf("%s","BlockAST\n");}
    llvm::Value *codegen() /*override*/;

    PrototypeAST *next, *content;
    void visit() override {}
};
struct ForAST : public BlockAST
{
    ForAST(ExprAST *s1, ExprAST *s2, ExprAST *s3, PrototypeAST *body);
    llvm::Value *codegen() /*override*/;

    ExprAST *header[3];
    PrototypeAST *body;
    void visit() override {}
};
struct IfAST : public PrototypeAST
{
    IfAST(ExprAST *det, BlockAST *ybranch, BlockAST *nbranch = NULL)
        : det(det), ybranch(ybranch), nbranch(nbranch), PrototypeAST("if") {}
    llvm::Value *codegen() /*override*/;

    ExprAST *det;
    BlockAST *ybranch, *nbranch;
    void visit() override {}
};

struct DecAST : public PrototypeAST
{
    DecAST(const std::string &op);
    llvm::Value *codegen() /*override*/;
    void visit() override {}
};

struct TypeAST : public PrototypeAST
{
    TypeAST(const std::string &op);
    llvm::Value *codegen() /*override*/;
    void visit() override {}
};
struct FunctionAST : public PrototypeAST
{
    FunctionAST(const std::string &op, TypeAST *type, StmtAST *body, DecAST *list)
        : PrototypeAST(op), type(type), body(body), args(args) {}
    llvm::Function *codegen() /*override*/;

    StmtAST *body;
    DecAST *args;
    TypeAST *type;
    void visit() override {}
};

struct ExprAST : public PrototypeAST
{
    ExprAST(const std::string &op) : PrototypeAST(op) {}
    virtual llvm::Value *codegen() = 0;
};
struct CommaExprAST : public ExprAST
{
    CommaExprAST(const std::string &op);
    llvm::Value *codegen() override;

    ExprAST *next;
    virtual void visit() = 0;
};
struct CallExprAST : public ExprAST
{
    CallExprAST(const std::string &op, ExprAST *args) : ExprAST(op), args(args) {printf("%s","CallExprAST\n");}
    llvm::Value *codegen() override;

    ExprAST *args;
    void visit() override {}
};
struct UnaryExprAST : public ExprAST
{
    UnaryExprAST(const std::string &op, ExprAST *exp) : exp(exp), ExprAST(op) {printf("%s","UnaryExprAST\n");}
    llvm::Value *codegen() override;

    ExprAST *exp;
    void visit() override {}
};
struct BinaryExprAST : public ExprAST
{
    BinaryExprAST(const std::string &op, ExprAST *lhs, ExprAST *rhs) : lhs(lhs), rhs(rhs), ExprAST(op) {printf("%s","BinaryExprAST\n");}
    llvm::Value *codegen() override;

    ExprAST *lhs, *rhs;
    void visit() override {}
};
struct LiteralAST : public ExprAST
{
    LiteralAST(const std::string &op, float val) : ExprAST(op), val(val) {printf("%s","LiteralAST\n");}
    llvm::Value *codegen() override;

    float val;
    void visit() override {}
};
struct VarAST : public ExprAST
{
    VarAST(const std::string &op) : ExprAST(op) {printf("%s","VarAST\n");}
    llvm::Value *codegen() override;

    char baseType;
    PrototypeAST *type;
    void visit() override {}
};
#endif