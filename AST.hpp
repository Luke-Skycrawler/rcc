#ifndef _AST_HPP
#define _AST_HPP
#include <iostream>
#include "CodeGen.h"
#include <vector>
#include <string>
#include <sstream>

extern bool type_error_alarm;
llvm::Function *CreateScanf();
llvm::Function *CreatePrintf();
extern std::map<std::string, std::string> binding_info_map;
extern std::map<std::string, llvm::AllocaInst*> bindings;

inline std::string INT2STRING(int x)
{
    std::stringstream ss;
    ss << x;
    return std::string(ss.str());
}

inline void PRINT_INDENT(int indent, std::string msg = "", bool new_line = 1)
{
    for (int i = 1; i < indent; i++)
        std::cout << "\t";
    if (indent)
        std::cout << "|___";
    if (new_line)
        std::cout << msg << std::endl;
    else
        std::cout << msg;
}

inline std::string GET_TYPE(std::string name)
{
    llvm::AllocaInst* inst = bindings[name];
    if(!inst) return "NULL";
    if(inst->getAllocatedType()->isIntegerTy())
    {
        int num_bit = inst->getAllocatedType()->getIntegerBitWidth();
        if(num_bit == 8) return "char";
        else if(num_bit == 32) return "int";
    }
    else if(inst->getAllocatedType()->isDoubleTy())
    {
        return "double";
    }
    return "NULL";
}

inline std::string TRANSLATE_ALLOCAINST2TYPE(llvm::AllocaInst* inst)
{
    if(!inst) return "NULL";
    if(inst->getAllocatedType()->isIntegerTy())
    {
        int num_bit = inst->getAllocatedType()->getIntegerBitWidth();
        if(num_bit == 8) return "char";
        else if(num_bit == 32) return "int";
    }
    else if(inst->getAllocatedType()->isDoubleTy())
    {
        return "double";
    }
    return "NULL";
}

// enum RCC_TYPE {RCC_CHAR = 1, RCC_INT = 2, RCC_DOUBLE = 3, RCC_STRING_LITERAL = 4, RCC_ERROR = -1, RCC_NULL = 0};

class Node;
class Nprogram;
class NexternalDeclaration;
class Ndeclaration;
class NinitDeclarator;
// class Ndeclarator;
class NdirectDeclarator;
class NparameterDeclaration;
class Ninitializer;
class NfunctionDefinition;
class NcompoundStatement;
class Nstatement;
class NexprStatement;
class NtypeSpecifier;
class Nexpr;
class NassignExpr;
class NcondExpr;
class NcastExpr;
class NunaryExpr;
class NpostfixExpr;
class Nidentifier;
class Nconstant;
class Nstruct;
class NderivedType;

/**
 * Base class of AST node, for derivation and inheritation
 */
class Node
{
public:
    virtual ~Node() {}
    virtual llvm::Value *codeGen() = 0;
    virtual void printNode(int indent){};
};

/**
 * `program` node -- the root node!
 * @param external_declaration_list: a vector of `external_declaration` nodes,
 * i.e., `program` consists of function definitions and declarations
 */
class Nprogram : public Node
{
public:
    Nprogram(std::vector<NexternalDeclaration *> &external_declaration_list) : external_declaration_list(external_declaration_list) {}
    void push_back(NexternalDeclaration *external_declaration)
    {
        external_declaration_list.push_back(external_declaration);
    }
    llvm::Value *codeGen();
    virtual void printNode(int indent);

private:
    std::vector<NexternalDeclaration *> external_declaration_list;
};

/**
 * `external_declaration` node -- either a declaration or a function-definition
 * It's a base virtual node for
 * - `Ndeclaration`
 * - `NfunctionDefinition`
 */
class NexternalDeclaration : public Node
{
};
class NderivedType : public Node
{
    NderivedType(char type);
    void push(Nexpr *element) { arraySize.push_back(element); }
    llvm::Value *codeGen();
    void printNode(int indent)
    {
        if (next)
            next->printNode(indent);
    }
    std::string baseType;
    std::vector<Nexpr *> arraySize;
    NderivedType *next; // used for function parameter list only
};
struct Nstruct : public Node
{
    // nearly the same as BlockAST
    Nstruct(const std::string &name, std::vector<Ndeclaration *> *content = NULL);
    std::string name;
    std::vector<Ndeclaration *> *content;
    llvm::Value *codeGen();
    void printNode(int indent);
};
struct NparameterList : public Node
{
    NparameterList(const std::string &name) : name(name) {}
    std::string name;
    NparameterList *next;
    llvm::Value *codeGen();
    void printNode(int indent)
    {
        PRINT_INDENT(indent, "");
        if (next)
            next->printNode(indent);
    }
};
/**
 * `declaration` node -- a declaration looks like 'int x = 3', consisting of
 * @param type_specifier: like type_specifier (TODO: storage_type like 'static' and type qualifier like 'const' should be implemented)
 * @param init_declarator_list
 */
class Ndeclaration : public NexternalDeclaration
{
public:
    Ndeclaration(NtypeSpecifier *type_specifier, std::vector<NinitDeclarator *> &init_declarator_list) : type_specifier(type_specifier),
                                                                                                         init_declarator_list(init_declarator_list) { }
    Ndeclaration(NtypeSpecifier *type_specifier) : type_specifier(type_specifier) { }
    llvm::Value *codeGen();
    void printNode(int indent);
    void bind();

private:
    NtypeSpecifier *type_specifier; // like 'int' in 'int x = 3'
    std::vector<NinitDeclarator *> init_declarator_list;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 * It's a base node for `NdirectDeclarator` to inherit!!!
 * The `initializer` contains the initializer part, while the other info are in the derived `NdirectDeclarator` node
 */
class NinitDeclarator : public Node
{
public:
    NinitDeclarator() {}
    llvm::Value *codeGen();
    virtual void printNode(int indent);
    virtual void bind(std::string type) { initializer = nullptr; }; // Do nothing
    Ninitializer *initializer;
};

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', 'x[30]', 'f(int x)', 'f(x)' or 'f()'
 */
class NdirectDeclarator : public NinitDeclarator
{
public:
    enum DIRECT_DECLARATOR_TYPE
    {
        IDENTIFIER = 0,
        SQUARE_BRACKET_CONSTANT = 1,
        SQUARE_BRACKET_EMPTY = 2, // probably not used
        PARENTHESES_PARAMETER_LIST = 3,
        PARENTHESES_IDENTIFIER_LIST = 4, // probably not used
        PARENTHESES_EMPTY = 5
    };
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, Nidentifier *identifier) : direct_declarator_type(direct_declarator_type),
                                                                                                identifier(identifier) {}
    llvm::Value *codeGen();
    virtual void printNode(int indent);
    void bind(std::string type);
    void pushIntConstant(Nconstant *int_constant);
    void updateType(DIRECT_DECLARATOR_TYPE direct_declarator_type);
    void setParameterList(const std::vector<NparameterDeclaration *> &parameter_list);
    void setIdentifierList(const std::vector<Nidentifier *> &identifier_list);
    // private:
    DIRECT_DECLARATOR_TYPE direct_declarator_type;
    Nidentifier *identifier;
    std::vector<NparameterDeclaration *> parameter_list;
    std::vector<Nidentifier *> identifier_list;
    std::vector<int> int_constant_list;
};

class NparameterDeclaration : public Node
{
public:
    NparameterDeclaration(NtypeSpecifier *type_specifier, NdirectDeclarator *direct_declarator) : type_specifier(type_specifier),
                                                                                                  direct_declarator(direct_declarator) {}
    NparameterDeclaration(NtypeSpecifier *type_specifier) : type_specifier(type_specifier) {}
    llvm::Value *codeGen();
    void printNode(int indent);

// private:
    NtypeSpecifier *type_specifier;
    NdirectDeclarator *direct_declarator;
};

/**
 * `initializer` node -- looks like '{3, x + y, P | Q}'
 * @param assign_expr: a single assign_expr (actually any expression with priviledge higher than the assignment expression)
 * @param initializer_list: a nested vector of initializer
 * (Could only be 1 of the 2!)
 */
class Ninitializer : public Node
{
public:
    Ninitializer(Nexpr *assign_expr) : assign_expr(assign_expr) {}
    Ninitializer(std::vector<Ninitializer *> &initializer_list) : initializer_list(initializer_list) {}
    llvm::Value *codeGen();
    virtual void printNode(int indent);

private:
    Nexpr *assign_expr;
    std::vector<Ninitializer *> initializer_list;
};

/**
 * `function_definition` node -- a function definition like 'int f(int x, double y, char z) {...}'
 * or maybe a function call?
 * @param type_specifier: 'int'
 * @param direct_declarator: 'f(int x, double y, char z)'
 * @param declaration_list: what for?
 * @param compound_statement: '{...}'
 */
class NfunctionDefinition : public NexternalDeclaration
{
public:
    NfunctionDefinition(NtypeSpecifier *type_specifier, NdirectDeclarator *direct_declarator, std::vector<Ndeclaration *> &declaration_list, NcompoundStatement *compound_statement) : type_specifier(type_specifier),
                                                                                                                                                                                       direct_declarator(direct_declarator),
                                                                                                                                                                                       declaration_list(declaration_list),
                                                                                                                                                                                       compound_statement(compound_statement) {}
    NfunctionDefinition(NtypeSpecifier *type_specifier, NdirectDeclarator *direct_declarator, NcompoundStatement *compound_statement) : type_specifier(type_specifier),
                                                                                                                                        direct_declarator(direct_declarator),
                                                                                                                                        compound_statement(compound_statement) {}
    NfunctionDefinition(NdirectDeclarator *direct_declarator, std::vector<Ndeclaration *> &declaration_list, NcompoundStatement *compound_statement) : direct_declarator(direct_declarator),
                                                                                                                                                       declaration_list(declaration_list),
                                                                                                                                                       compound_statement(compound_statement) {}
    NfunctionDefinition(NdirectDeclarator *direct_declarator, NcompoundStatement *compound_statement) : direct_declarator(direct_declarator),
                                                                                                        compound_statement(compound_statement) {}
    llvm::Value *codeGen();
    void printNode(int indent);
    void bind();
    // private:
    NtypeSpecifier *type_specifier;               // 'int'
    NdirectDeclarator *direct_declarator;         // 'f(int x, double y, char z)'
    std::vector<Ndeclaration *> declaration_list; // what for?
    NcompoundStatement *compound_statement;       // '{...}'
};

/**
 * `statement` node -- a base class for `statement`
 */
class Nstatement : public Node
{
public:
    llvm::Value *codeGen();
};

/**
 * `compound_statement` node -- a compound statement, consisting of
 * @param statement_list: a statement list
 * @param declaration_list: a declaration list
 */
class NcompoundStatement : public Nstatement
{
public:
    NcompoundStatement(const std::vector<Ndeclaration *> &declaration_list, const std::vector<Nstatement *> &statement_list) : statement_list(statement_list),
                                                                                                                               declaration_list(declaration_list) {}
    NcompoundStatement(const std::vector<Ndeclaration *> &declaration_list) : declaration_list(declaration_list) {}
    NcompoundStatement(const std::vector<Nstatement *> &statement_list) : statement_list(statement_list) {}
    NcompoundStatement() {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    std::vector<Ndeclaration *> declaration_list;
    std::vector<Nstatement *> statement_list;
};

/**
 * `expr_statement` node -- an expression statement
 * It's containing 0 or 1 `expr`...
 * @param expr: a pointer to a `expr` or just a nullptr
 */
class NexprStatement : public Nstatement
{
public:
    NexprStatement() { expr = nullptr; }
    NexprStatement(Nexpr *expr) : expr(expr) {}
    llvm::Value *codeGen();
    void printNode(int indent);

protected:
    Nexpr *expr;
};

/**
 * `if_statement` node -- an if-else statement
 * @param cond_expr
 * @param if_statement
 * @param else_statement
 */
class NifStatement : public Nstatement
{
public:
    NifStatement(Nexpr *cond_expr, Nstatement *if_statement) : cond_expr(cond_expr),
                                                               if_statement(if_statement) {}
    NifStatement(Nexpr *cond_expr, Nstatement *if_statement, Nstatement *else_statement) : cond_expr(cond_expr),
                                                                                           if_statement(if_statement),
                                                                                           else_statement(else_statement) {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    Nexpr *cond_expr;           // the condition expression
    Nstatement *if_statement;   // IF-statement
    Nstatement *else_statement; // ELSE-statement
};

/**
 * `type_specifier` node -- 'char', 'int' or 'double'
 * @param type: a std::string "char", "int" or "double"
 */
class NtypeSpecifier : public Node
{
public:
    NtypeSpecifier(std::string type) : type(type) {}
    llvm::Value *codeGen();
    virtual void printNode(int indent);
    // private:
    std::string type;
};

/**
 * `expr` node -- an expression looks like 'x = 3, ++y, d[i]--'
 * The class would be inherited by different types of expression classes,
 * while it holds a vector...
 * TODO: The design costs extra non-used space for derived classes, which is not so good...
 * But the current design is plained and conforms to our EBNF!
 */
class Nexpr : public Node
{
public:
    Nexpr() {} // default constructor
    Nexpr(std::vector<Nexpr *> &expr_list) : expr_list(expr_list) {}
    void push_back(Nexpr *expr)
    {
        expr_list.push_back(expr);
    }
    void printNode(int indent);
    llvm::Value *codeGen() = 0;
    // virtual std::string getType() { return "NULL"; }
    std::string type;
private:
    std::vector<Nexpr *> expr_list;
};

/**
 * x = 3
 */
class NassignExpr : public Nexpr
{
public:
    NassignExpr(Nexpr *unary_expr, std::string assign_op, Nexpr *assign_expr) : unary_expr(unary_expr),
                                                                                assign_op(assign_op),
                                                                                assign_expr(assign_expr)
    {
        type = "NULL";
    }
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    Nexpr *unary_expr;
    std::string assign_op;
    Nexpr *assign_expr;
};

/**
 * P ? 0 : 1
 */
class NcondExpr : public Nexpr
{
public:
    NcondExpr(Nexpr *logical_or_expr, Nexpr *expr, Nexpr *cond_expr) : logical_or_expr(logical_or_expr),
                                                                       expr(expr),
                                                                       cond_expr(cond_expr) {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    Nexpr *logical_or_expr;
    Nexpr *expr;
    Nexpr *cond_expr;
};

/**
 * P op Q
 */
class NbinaryExpr : public Nexpr
{
public:
    NbinaryExpr(const std::string &op, Nexpr *lhs, Nexpr *rhs) : op(op), lhs(lhs), rhs(rhs)
    {
        type = "NULL";
    }
    std::string op;
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    Nexpr *lhs, *rhs;
};

/**
 * (int)x
 */
class NcastExpr : public Nexpr
{
public:
    NcastExpr(NtypeSpecifier *type_specifier, Nexpr *cast_expr) : type_specifier(type_specifier),
                                                                  cast_expr(cast_expr) {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    NtypeSpecifier *type_specifier;
    Nexpr *cast_expr;
};

/**
 * ++x or --x or sizeof(x)
 * or
 * ~x (or ~(int)x) or sizeof(int)
 * TODO: maybe we can combine `unary_expr` and `cast_expr` into one member (or use `union`)!
 */
class NunaryExpr : public Nexpr
{
public:
    enum UNARY_OP
    {
        INC_OP = 0,        // followed by a unary_expr
        DEC_OP = 1,        // followed by a unary_expr
        SIZEOF_U = 2,      // followed by a unary_expr
        SIZEOF_T = 3,      // followed by a type_specifier
        AND_OP = 4,        // followed by a cast_expr
        MULTIPLY_OP = 5,   // followed by a cast_expr
        PLUS_OP = 6,       // followed by a cast_expr
        MINUS_OP = 7,      // followed by a cast_expr
        TILDE_OP = 8,      // followed by a cast_expr
        EXCLAMATION_OP = 9 // followed by a cast_expr
    };
    NunaryExpr(UNARY_OP unary_op, Nexpr *unary_expr, Nexpr *cast_expr) : unary_op(unary_op),
                                                                         unary_expr(unary_expr),
                                                                         cast_expr(cast_expr) {}
    NunaryExpr(UNARY_OP unary_op, NtypeSpecifier *type_specifier) : unary_op(unary_op),
                                                                    type_specifier(type_specifier) {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    UNARY_OP unary_op;
    //Only one of the following should be valid!
    Nexpr *unary_expr;
    Nexpr *cast_expr;
    NtypeSpecifier *type_specifier;
};

/**
 * x[3] or f() or f(3, 4) or a.id or a->id or x++ or x--
 */
class NpostfixExpr : public Nexpr
{
public:
    enum POSTFIX_TYPE
    {
        SQUARE_BRACKETS = 0,
        PARENTHESES = 1,
        DOT = 2,
        PTR_OP = 3,
        INC_OP = 4,
        DEC_OP = 5
    };
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr *postfix_expr, Nexpr *expr) : postfix_expr(postfix_expr),
                                                                                postfix_type(postfix_type),
                                                                                expr(expr) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr *postfix_expr, std::vector<Nexpr *> &argument_expr_list) : postfix_expr(postfix_expr),
                                                                                                             postfix_type(postfix_type),
                                                                                                             argument_expr_list(argument_expr_list) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr *postfix_expr, Nidentifier *identifier) : postfix_expr(postfix_expr),
                                                                                            postfix_type(postfix_type),
                                                                                            identifier(identifier) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr *postfix_expr) : postfix_expr(postfix_expr),
                                                                   postfix_type(postfix_type) {}
    llvm::Value *codeGen();
    void printNode(int indent);

private:
    POSTFIX_TYPE postfix_type;
    Nexpr *postfix_expr;
    Nexpr *expr;                             // valid when postfix_type = SUQARE_BRACKETS
    std::vector<Nexpr *> argument_expr_list; // valid when postfix_type = PARENTHESES, could be a nullptr!
    Nidentifier *identifier;                 // valid when postfix_type = DOT or PTR_OP
    // when postfix_type = INC_OP or DEC_OP, none of the 3 upon would be used!
};

/**
 * `IDENTIFIER` node -- an identifier
 * @param name: a std::string variable for the identifier's name
 */
class Nidentifier : public Nexpr
{
public:
    Nidentifier(std::string &name) : name(name) {}
    llvm::Value *codeGen();
    void printNode(int indent);
    // private:
    std::string name;
};

/**
 * `CONSTANT` node -- a constant
 * @param type: a std::string "char", "int" or "double"
 */
class Nconstant : public Nexpr
{
public:
    Nconstant(const std::string &type, char value) { this->type=type;this->value.char_value = value; }
    Nconstant(const std::string &type, int value) { this->type=type;this->value.int_value = value; }
    Nconstant(const std::string &type, double value) { this->type=type;this->value.double_value = value; }
    Nconstant(const std::string &type, char *value);
    llvm::Value *codeGen();
    void printNode(int indent);
    // private:
    union Value
    {
        char char_value;
        int int_value;
        double double_value;
        char *string_literal_value;
    } value;
};
class NreturnStatement : public NexprStatement{
public:
    NreturnStatement(Nexpr *expr=NULL) : NexprStatement(expr) {}
    llvm::Value *codeGen() override;
};
inline void error(const char *msg)
{
    printf("error:%s \n", msg);
    exit(1);
}

#endif