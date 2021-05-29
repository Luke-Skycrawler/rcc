#ifndef _AST_H
#define _AST_H
#include "codegen.h"
struct ExprAST;
struct PrototypeAST;
extern PrototypeAST *root;
struct PrototypeAST
{
    std::string op; // for drawing
    PrototypeAST(const std::string &op) : op(op) { printf("\n"); }
    virtual llvm::Value *codegen() = 0;
    bool isLeaf;
    virtual void traverse() = 0;

protected:
    virtual void visit(void *_child1, void *_child2 = NULL, void *_sibling = NULL)
    {
        PrototypeAST *sibling = (PrototypeAST *)_sibling,
                     *child2  = (PrototypeAST *)_child2,
                     *child1  = (PrototypeAST *)_child1;
        if (child1)
            child1->traverse();
        if (child2)
            child2->traverse();
        // ---------------
        // reserved for visualizing
        if (sibling)
            sibling->traverse();
    }
};
struct StmtAST : public PrototypeAST
{
    StmtAST(ExprAST *expr = NULL, PrototypeAST *next = NULL) : PrototypeAST(";"), expr(expr), next(next) { printf("%s", "StmtAST\n"); }
    llvm::Value *codegen() /*override*/;

    ExprAST *expr;
    PrototypeAST *next;
    void traverse() override
    {
        visit(expr, NULL, next);
    }
};
struct BlockAST : public PrototypeAST
{
    BlockAST(PrototypeAST *content = NULL, PrototypeAST *next = NULL)
        : PrototypeAST("{"), content(content), next(next) { printf("%s", "BlockAST\n"); }
    llvm::Value *codegen() /*override*/;

    PrototypeAST *next, *content;
    void traverse() override
    {
        visit(content, NULL, next);
    }
};
struct ForAST : public BlockAST
{
    ForAST(ExprAST *s1, ExprAST *s2, ExprAST *s3, PrototypeAST *body);
    llvm::Value *codegen() /*override*/;

    ExprAST *header[3];
    PrototypeAST *body;
    void traverse() override
    {
        // visit (header[0],NULL );
        // FIXME:
    }
};
struct IfAST : public PrototypeAST
{
    IfAST(ExprAST *det, BlockAST *ybranch, BlockAST *nbranch = NULL)
        : det(det), ybranch(ybranch), nbranch(nbranch), PrototypeAST("if") {}
    llvm::Value *codegen() /*override*/;

    ExprAST *det;
    BlockAST *ybranch, *nbranch;
    void traverse() override { visit(det, ybranch, NULL); }
    // FIXME:
};

struct DecAST : public PrototypeAST
{
    DecAST(const std::string &op);
    llvm::Value *codegen() /*override*/;
    void traverse() override { visit(NULL); }
    //
};

struct TypeAST : public PrototypeAST
{
    TypeAST(const std::string &op);
    llvm::Value *codegen() /*override*/;
    void traverse() override { visit(NULL); }
    //
};
struct FunctionAST : public PrototypeAST
{
    FunctionAST(BlockAST *body, TypeAST *type = NULL, DecAST *list = NULL)
        : PrototypeAST("()"), type(type), body(body), args(list) { printf("Defined Function %s", op.data()); }
    llvm::Value *codegen() /*override*/;

    BlockAST *body;
    DecAST *args;
    TypeAST *type;
    void traverse() override { visit(args, body); }
    // FIXME:
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
    virtual void traverse() = 0;
};
struct CallExprAST : public ExprAST
{
    CallExprAST(const std::string &op, ExprAST *args) : ExprAST(op), args(args) { printf("%s", "CallExprAST\n"); }
    llvm::Value *codegen() override;

    ExprAST *args;
    void traverse() override { visit(args); }
};
struct UnaryExprAST : public ExprAST
{
    UnaryExprAST(const std::string &op, ExprAST *expr) : expr(expr), ExprAST(op) { printf("%s", "UnaryExprAST\n"); }
    llvm::Value *codegen() override;

    ExprAST *expr;
    void traverse() override { visit(expr); }
};
struct BinaryExprAST : public ExprAST
{
    BinaryExprAST(const std::string &op, ExprAST *lhs, ExprAST *rhs) : lhs(lhs), rhs(rhs), ExprAST(op) { printf("%s", "BinaryExprAST\n"); }
    llvm::Value *codegen() override;

    ExprAST *lhs, *rhs;
    void traverse() override { visit(lhs, rhs, NULL); }
};
struct LiteralAST : public ExprAST
{
    LiteralAST(const std::string &op, float val) : ExprAST(op), val(val) { printf("%s", "LiteralAST\n"); }
    llvm::Value *codegen() override;

    float val;
    void traverse() override { printf("literal: %f\n", val); }
};
struct VarAST : public ExprAST
{
    VarAST(const std::string &op) : ExprAST(op) { printf("%s", "VarAST\n"); }
    llvm::Value *codegen() override;

    char baseType;
    PrototypeAST *type;
    void traverse() override { printf("type %c var%s", baseType, op.data()); }
};
#endif