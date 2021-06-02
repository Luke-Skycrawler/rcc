// FIXME: destructor get virtual

#ifndef _AST_H
#define _AST_H
#include "codegen.h"
#include "RccGlobal.hpp"
#include <vector>
struct ExprAST;
struct PrototypeAST;
struct DecAST;
struct VarAST;

extern std::vector<PrototypeAST *> root;
// extern std::map<std::string,StructAST*> structBindings;
extern int indent;
llvm::Function *CreateScanf();
llvm::Function *CreatePrintf();
struct PrototypeAST
{
    std::string op; // for drawing
    PrototypeAST(const std::string &op) : op(op)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("\n");
#endif
    }
    virtual llvm::Value *codegen() = 0;
    bool isLeaf;
    virtual void traverse() = 0;
};
inline void visit(void *_child1, void *_child2 = NULL, void *_sibling = NULL)
{
    PrototypeAST *sibling = static_cast<PrototypeAST *>(_sibling),
                 *child2 = static_cast<PrototypeAST *>(_child2),
                 *child1 = static_cast<PrototypeAST *>(_child1);
    indent++;
    if (child1)
        child1->traverse();
    if (child2)
        child2->traverse();
    indent--;
    // ---------------
    // reserved for visualizing
    if (sibling)
        sibling->traverse();
}
inline void print(int indent, const std::string &op)
{
    for (int i = 1; i < indent; i++)
        printf("  ");
    // for(int i=0;i<indent;i++)printf("__");
    if (indent)
        printf("|_");
    printf("%s\n", op.data());
}
struct StmtAST : public PrototypeAST
{
    StmtAST(ExprAST *expr = NULL, PrototypeAST *next = NULL) : PrototypeAST(";"), expr(expr), next(next)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "StmtAST\n");
#endif
    }
    llvm::Value *codegen() /*override*/;

    ExprAST *expr;
    PrototypeAST *next;
    void traverse() override
    {
        print(indent, op);
        visit(expr, NULL, next);
    }
};
struct BlockAST : public PrototypeAST
{
    BlockAST(PrototypeAST *content = NULL, PrototypeAST *next = NULL)
        : PrototypeAST("{"), content(content), next(next)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "BlockAST\n");
#endif
    }
    llvm::Value *codegen() /*override*/;

    PrototypeAST *next, *content;
    void traverse() override
    {
        print(indent, op);
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
    void traverse() override
    {
        print(indent, op);
        visit(det, ybranch, NULL);
    }
    // FIXME:
};
struct DerivedTypeAST : public PrototypeAST
{
//     DerivedTypeAST(const std::string &op) : PrototypeAST(op)
//     {
// #ifdef _SHOW_CONSTRUCTOR
//         printf("DerivedType\n");
// #endif
//     }
    DerivedTypeAST(char type);
    void push(ExprAST *element) { arraySize.push_back(element); }
    llvm::Value *codegen();
    void traverse() override
    {
        print(indent, op);
        visit(NULL, NULL, next);
    }
    std::vector<ExprAST *> arraySize;
    DerivedTypeAST *next; // used for function parameter list only
    // VarAST *parameterAlias; // for funtion definition
};
struct StructAST : public PrototypeAST
{
    // nearly the same as BlockAST
    StructAST(const std::string &op, DecAST *content = NULL);
    llvm::Value *codegen() /*override*/ { return NULL; }

    DecAST *content;
    void traverse() override
    {
        print(indent, op+"{");
        visit(content);
    }
};
struct ParameterListAST: public PrototypeAST {
    ParameterListAST(const std::string &op):PrototypeAST(op){}
    llvm::Value *codegen() /*override*/;
    ParameterListAST *next;
    void traverse() override
    {
        print(indent, op);
        visit(NULL,NULL,next);
    }
};
struct DecAST : public PrototypeAST
{
    // DecAST(const std::string &op);
    DecAST(const Buffer &buf) : PrototypeAST(buf.val.u8), baseType(buf.type), fullType(buf.type)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "DecAST\n");
#endif
    }

    llvm::Value *codegen() /*override*/;
    void traverse() override
    {
        print(indent, op);
        visit(NULL, NULL, next);
    }

    PrototypeAST *next;
    ExprAST *init;
    DerivedTypeAST fullType;
    char baseType;
    DecAST *tail()
    {
        if (!next)
            return this;
        auto p = dynamic_cast<DecAST *>(next);

        while (p->next)
        {
            p = dynamic_cast<DecAST *>(p->next);
        }
        return p;
    }
};

// struct TypeAST : public PrototypeAST
// {
//     TypeAST(const std::string &op);
//     llvm::Value *codegen() /*override*/;
//     void traverse() override
//     {
//         print(indent, op);
//         visit(NULL);
//     }
//     //
// };
struct FunctionAST : public PrototypeAST
{
    FunctionAST(BlockAST *body, const std::string &op = "", DerivedTypeAST *retType = NULL, DerivedTypeAST *list = NULL)
        : PrototypeAST(op + "()"), retType(retType), body(body), argsDec(list)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("Defined Function %s", op.data());
#endif
    }
    FunctionAST(BlockAST *body, const std::string &op, DecAST *list, DerivedTypeAST *retType = NULL)
        : PrototypeAST(op + "()"), retType(retType), body(body), argsImplement(list)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("Defined Function %s", op.data());
#endif
    }
    llvm::Value *codegen() /*override*/;

    BlockAST *body;
    DerivedTypeAST *argsDec, *retType;
    DecAST *argsImplement;
    void traverse() override
    {
        print(indent, op);
        if (argsDec)
            visit(argsDec, body);
        else
            visit(argsImplement, body);
    }
    // FIXME:
};
struct ExprAST : public PrototypeAST
{
    ExprAST(const std::string &op, char compatible = 'd') : PrototypeAST(op), compatible(compatible) {}
    char compatible; // least compatible type
    virtual llvm::Value *codegen() = 0;
    ExprAST *next; // only used in a string of comma expressions
};
// struct CommaExprAST : public ExprAST
// {
//     CommaExprAST(const std::string &op);
//     llvm::Value *codegen() override;

//     ExprAST *next;
//     virtual void traverse() = 0;
// };
struct CallExprAST : public ExprAST
{
    CallExprAST(const std::string &op, ExprAST *args = NULL) : ExprAST(op), args(args)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "CallExprAST\n");
#endif
    }
    llvm::Value *codegen() override;

    ExprAST *args;
    void traverse() override
    {
        print(indent, op + "()");
        visit(args, NULL, next);
    }
};
struct UnaryExprAST : public ExprAST
{
    UnaryExprAST(const std::string &op, ExprAST *expr) : expr(expr), ExprAST(op)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "UnaryExprAST\n");
#endif
    }
    llvm::Value *codegen() override;

    ExprAST *expr;
    void traverse() override
    {
        print(indent, op);
        visit(expr, NULL, next);
    }
};
struct BinaryExprAST : public ExprAST
{
    BinaryExprAST(const std::string &op, ExprAST *lhs, ExprAST *rhs) : lhs(lhs), rhs(rhs), ExprAST(op)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "BinaryExprAST\n");
#endif
    }
    llvm::Value *codegen() override;

    ExprAST *lhs, *rhs;
    void traverse() override
    {
        print(indent, op);
        visit(lhs, rhs, next);
    }
};
struct AssignAST : public ExprAST
{
    AssignAST(const std::string op, VarAST *lhs, ExprAST *rhs) : lhs(lhs), rhs(rhs), ExprAST(op) {}
    llvm::Value *codegen() override;
    VarAST *lhs;
    ExprAST *rhs;
    void traverse() override
    {
        print(indent, op);
        visit(lhs, rhs, next);
    }
};
struct LiteralAST : public ExprAST
{
    /* LiteralAST(const std::string &op, float val) : ExprAST(op), val(val) {
    #ifdef _SHOW_CONSTRUCTOR
        printf("%s", "LiteralAST\n");
    #endif
    }
    LiteralAST(const std::string &op):ExprAST(op){
    #ifdef _SHOW_CONSTRUCTOR
    printf("String Literal\n");
    #endif
    }*/
    LiteralAST(const rccGlobal &global) : ExprAST(global.buf.advice == 's' ? global.buf.val.u8 : "foo", global.buf.advice)
    {
        if (global.buf.advice != 's')
            val = global.buf.val.f64;
    }
    llvm::Value *codegen() override;

    double val;
    void traverse() override
    {
        if (op != "foo")
            print(indent, op);
        else
            print(indent, "literal: val = " + std::to_string(val));
        visit(NULL, NULL, next);
    }
};
struct VarAST : public ExprAST
{
    VarAST(const std::string &op, char baseType = ' ') : ExprAST(op), baseType(baseType)
    {
#ifdef _SHOW_CONSTRUCTOR
        printf("%s", "VarAST\n");
#endif
    }
    llvm::Value *codegen() override;
    char baseType;
    PrototypeAST *type;
    void traverse() override
    {
        print(indent, "var " + op + ": type = " + baseType);
        visit(NULL, NULL, next);
    }
};
#endif