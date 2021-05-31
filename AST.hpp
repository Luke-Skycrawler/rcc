#ifndef _AST_HPP
#define _AST_HPP
#include <iostream>
#include "CodeGen.h"
#include <vector>
#include <string>

inline void PRINT_INDENT(int indent, std::string msg = "", bool new_line = 1)
{
    for (int i = 1; i < indent; i++)
        std::cout << "\t";
    if (indent)
        std::cout << "|___";
    if(new_line) std::cout << msg << std::endl;
    else std::cout << msg;
}

enum RCC_TYPE {RCC_CHAR = 1, RCC_INT = 2, RCC_DOUBLE = 3};

class Node;
class Nprogram;
class NexternalDeclaration;
class Ndeclaration;
class NdeclarationSpecifiers;
class NinitDeclarator;
class Ndeclarator;
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
class NlogicalOrExpr;
class NlogicalAndExpr;
class NinclusiveOrExpr;
class NexclusiveOrExpr;
class NandExpr;
class NequalityExpr;
class NrelationalExpr;
class NshiftExpr;
class NadditiveExpr;
class NmultiplicativeExpr;
class NcastExpr;
class NunaryExpr;
class NpostfixExpr;
class NprimaryExpr;
class Nidentifier;
class Nconstant;

/**
 * Base class of AST node, for derivation and inheritation
 */
class Node {
public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen()=0;
    virtual void printNode(int indent) {};
};

/**
 * `program` node -- the root node!
 * @param external_declaration_list: a vector of `external_declaration` nodes,
 * i.e., `program` consists of function definitions and declarations
 */
class Nprogram: public Node {
public:
    Nprogram(std::vector<NexternalDeclaration*>& external_declaration_list):\
        external_declaration_list(external_declaration_list) {}
    void push_back(NexternalDeclaration* external_declaration)
    {
        external_declaration_list.push_back(external_declaration);
    }
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    std::vector<NexternalDeclaration*> external_declaration_list;
};

/**
 * `external_declaration` node -- either a declaration or a function-definition
 * It's a base virtual node for
 * - `Ndeclaration`
 * - `NfunctionDefinition`
 */
class NexternalDeclaration: public Node {
};

/**
 * `declaration` node -- a declaration looks like 'int x = 3', consisting of
 * @param declaration_specifiers: like type_specifier (TODO: storage_type like 'static' and type qualifier like 'const' should be implemented)
 * @param init_declarator_list
 */
class Ndeclaration: public NexternalDeclaration {
public:
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers, std::vector<NinitDeclarator*>& init_declarator_list):\
        declaration_specifiers(declaration_specifiers),
        init_declarator_list(init_declarator_list) {}
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers):\
        declaration_specifiers(declaration_specifiers) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    NdeclarationSpecifiers* declaration_specifiers; // like 'int' in 'int x = 3'
    std::vector<NinitDeclarator*> init_declarator_list;
};

/**
 * `declaration_specifiers` node -- a declaration specifer looks like `static int` or `int` or `const int`
 * TODO: implement it with full feature
 */
class NdeclarationSpecifiers: public Node {
public:
    NdeclarationSpecifiers(NtypeSpecifier* type_specifier):type_specifier(type_specifier) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    NtypeSpecifier* type_specifier;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 */
class NinitDeclarator: public Node {
public:
    NinitDeclarator(Ndeclarator* declarator, Ninitializer* initializer):declarator(declarator), initializer(initializer) {}
    NinitDeclarator(Ndeclarator* declarator):declarator(declarator) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    Ndeclarator* declarator;
    Ninitializer* initializer;
};

/**
 * `declarator` node -- a declarator looks like 'x', '(x)', 'x[30]' or '*x'
 * TODO: implement the pointer feature like '*x'
 */
class Ndeclarator: public Node {
public:
    Ndeclarator(NdirectDeclarator* direct_declarator):direct_declarator(direct_declarator) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    NdirectDeclarator* direct_declarator;
};

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', '(x)' or 'x[30]'
 * TODO: only a single identifier like 'x' is implemented now, more to implement later
 */
class NdirectDeclarator: public Node {
public:
    enum DIRECT_DECLARATOR_TYPE {
        IDENTIFIER = 0,
        NESTED_DECLARATOR = 1,
        SQUARE_BRACKET_CONSTANT = 2,
        SQUARE_BRACKET_EMPTY = 3,
        PARENTHESES_PARAMETER_LIST = 4,
        PARENTHESES_IDENTIFIER_LIST = 5,
        PARENTHESES_EMPTY = 6
    };
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, Nidentifier* identifier):\
        direct_declarator_type(direct_declarator_type),
        identifier(identifier) {}
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, Ndeclarator* declarator):\
        direct_declarator_type(direct_declarator_type),
        declarator(declarator) {}
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, NdirectDeclarator* direct_declarator, Nconstant* int_constant):\
        direct_declarator_type(direct_declarator_type),
        direct_declarator(direct_declarator),
        int_constant(int_constant) {}
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, NdirectDeclarator* direct_declarator):\
        direct_declarator_type(direct_declarator_type),
        direct_declarator(direct_declarator) {}
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, NdirectDeclarator* direct_declarator, std::vector<NparameterDeclaration*>& parameter_list):\
        direct_declarator_type(direct_declarator_type),
        direct_declarator(direct_declarator),
        parameter_list(parameter_list) {}
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, NdirectDeclarator* direct_declarator, std::vector<Nidentifier*>& identifier_list):\
        direct_declarator_type(direct_declarator_type),
        direct_declarator(direct_declarator),
        identifier_list(identifier_list) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    DIRECT_DECLARATOR_TYPE direct_declarator_type;
    Nidentifier* identifier;
    Ndeclarator* declarator;
    Nconstant* int_constant;
    NdirectDeclarator* direct_declarator;
    std::vector<NparameterDeclaration*> parameter_list;
    std::vector<Nidentifier*> identifier_list;
};

class NparameterDeclaration: public Node {
public:
    NparameterDeclaration(NdeclarationSpecifiers* declaration_specifiers, Ndeclarator* declarator):
        declaration_specifiers(declaration_specifiers),
        declarator(declarator) {}
    NparameterDeclaration(NdeclarationSpecifiers* declaration_specifiers):
        declaration_specifiers(declaration_specifiers) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    NdeclarationSpecifiers* declaration_specifiers;
    Ndeclarator* declarator;
};

/**
 * `initializer` node -- looks like '{3, x + y, P | Q}'
 * @param assign_expr: a single assign_expr (actually any expression with priviledge higher than the assignment expression)
 * @param initializer_list: a nested vector of initializer
 * (Could only be 1 of the 2!)
 */
class Ninitializer: public Node {
public:
    Ninitializer(Nexpr* assign_expr):assign_expr(assign_expr) {}
    Ninitializer(std::vector<Ninitializer*>& initializer_list):initializer_list(initializer_list) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    Nexpr* assign_expr;
    std::vector<Ninitializer*> initializer_list;
};

/**
 * `function_definition` node -- a function definition like 'int f(int x, double y, char z) {...}'
 * or maybe a function call?
 * @param declaration_specifiers: 'int'
 * @param declarator: 'f(int x, double y, char z)'
 * @param declaration_list: what for?
 * @param compound_statement: '{...}'
 */
class NfunctionDefinition: public NexternalDeclaration {
public:
    NfunctionDefinition(NdeclarationSpecifiers* declaration_specifiers, Ndeclarator* declarator, std::vector<Ndeclaration*>& declaration_list, NcompoundStatement* compound_statement):\
        declaration_specifiers(declaration_specifiers),
        declarator(declarator),
        declaration_list(declaration_list),
        compound_statement(compound_statement) {}
    NfunctionDefinition(NdeclarationSpecifiers* declaration_specifiers, Ndeclarator* declarator, NcompoundStatement* compound_statement):\
        declaration_specifiers(declaration_specifiers),
        declarator(declarator),
        compound_statement(compound_statement) {}
    NfunctionDefinition(Ndeclarator* declarator, std::vector<Ndeclaration*>& declaration_list, NcompoundStatement* compound_statement):\
        declarator(declarator),
        declaration_list(declaration_list),
        compound_statement(compound_statement) {}
    NfunctionDefinition(Ndeclarator* declarator, NcompoundStatement* compound_statement):\
        declarator(declarator),
        compound_statement(compound_statement) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    NdeclarationSpecifiers* declaration_specifiers; // 'int'
    Ndeclarator* declarator; // 'f(int x, double y, char z)'
    std::vector<Ndeclaration*> declaration_list; // what for?
    NcompoundStatement* compound_statement; // '{...}'
};

/**
 * `statement` node -- a base class for `statement`
 */
class Nstatement: public Node {
public:
    llvm::Value* codeGen();
};

/**
 * `compound_statement` node -- a compound statement, consisting of
 * @param statement_list: a statement list
 * @param declaration_list: a declaration list
 */
class NcompoundStatement: public Nstatement {
public:
    NcompoundStatement(const std::vector<Ndeclaration*>& declaration_list,const std::vector<Nstatement*>& statement_list):\
        statement_list(statement_list),
        declaration_list(declaration_list) {}
    NcompoundStatement(const std::vector<Ndeclaration*>& declaration_list):declaration_list(declaration_list) {}
    NcompoundStatement(const std::vector<Nstatement*>& statement_list):statement_list(statement_list) {}
    NcompoundStatement() {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    std::vector<Ndeclaration*> declaration_list;
    std::vector<Nstatement*> statement_list;
};

/**
 * `expr_statement` node -- an expression statement
 * It's containing 0 or 1 `expr`...
 * @param expr: a pointer to a `expr` or just a nullptr
 */
class NexprStatement: public Nstatement {
public:
    NexprStatement() {expr = nullptr;}
    NexprStatement(Nexpr* expr):expr(expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* expr;
};

/**
 * `type_specifier` node -- 'char', 'int' or 'double'
 * @param type: RCC_CHAR, RCC_INT or RCC_DOUBLE (enum type)
 */
class NtypeSpecifier: public Node {
public:
    NtypeSpecifier(RCC_TYPE type):type(type) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
private:
    RCC_TYPE type;
};

/**
 * `expr` node -- an expression looks like 'x = 3, ++y, d[i]--'
 * The class would be inherited by different types of expression classes,
 * while it holds a vector...
 * TODO: The design costs extra non-used space for derived classes, which is not so good...
 * But the current design is plained and conforms to our EBNF!
 */
class Nexpr: public Node {
public:
    Nexpr() {} // default constructor
    Nexpr(std::vector<Nexpr*>& expr_list):expr_list(expr_list) {}
    void push_back(Nexpr* expr) {
        expr_list.push_back(expr);
    }
    void printNode(int indent);
    llvm::Value* codeGen();
private:
    std::vector<Nexpr*> expr_list;
};

/**
 * x = 3
 */
class NassignExpr: public Nexpr {
public:
    enum ASSIGN_OP_TYPE {
        EQUAL = 0,
        MUL_ASSIGN = 1,
        DIV_ASSIGN = 2,
        MOD_ASSIGN = 3,
        ADD_ASSIGN = 4,
        SUB_ASSIGN = 5,
        LEFT_ASSIGN = 6,
        RIGHT_ASSIGN = 7,
        AND_ASSIGN = 8,
        XOR_ASSIGN = 9,
        OR_ASSIGN = 10
    };
    NassignExpr(Nexpr* unary_expr, ASSIGN_OP_TYPE assign_op, Nexpr* assign_expr):\
        unary_expr(unary_expr),
        assign_op(assign_op),
        assign_expr(assign_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* unary_expr;
    ASSIGN_OP_TYPE assign_op;
    Nexpr* assign_expr;
};

/**
 * P ? 0 : 1
 */
class NcondExpr: public Nexpr {
public:
    NcondExpr(Nexpr* logical_or_expr, Nexpr* expr, Nexpr* cond_expr):\
        logical_or_expr(logical_or_expr),
        expr(expr),
        cond_expr(cond_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* logical_or_expr;
    Nexpr* expr;
    Nexpr* cond_expr;
};

/**
 * P || Q
 */
class NlogicalOrExpr: public Nexpr {
public:
    NlogicalOrExpr(Nexpr* logical_or_expr, Nexpr* logical_and_expr):\
        logical_or_expr(logical_or_expr),
        logical_and_expr(logical_and_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* logical_or_expr;
    Nexpr* logical_and_expr;
};

/**
 * P && Q
 */
class NlogicalAndExpr: public Nexpr {
public:
    NlogicalAndExpr(Nexpr* logical_and_expr, Nexpr* inclusive_or_expr):\
        logical_and_expr(logical_and_expr),
        inclusive_or_expr(inclusive_or_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* logical_and_expr;
    Nexpr* inclusive_or_expr;
};

/**
 * P | Q
 */
class NinclusiveOrExpr: public Nexpr {
public:
    NinclusiveOrExpr(Nexpr* inclusive_or_expr, Nexpr* exclusive_or_expr):\
        inclusive_or_expr(inclusive_or_expr),
        exclusive_or_expr(exclusive_or_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* inclusive_or_expr;
    Nexpr* exclusive_or_expr;
};

/**
 * P ^ Q
 */
class NexclusiveOrExpr: public Nexpr {
public:
    NexclusiveOrExpr(Nexpr* exclusive_or_expr, Nexpr* and_expr):\
        exclusive_or_expr(exclusive_or_expr),
        and_expr(and_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* exclusive_or_expr;
    Nexpr* and_expr;
};

/**
 * P & Q
 */
class NandExpr: public Nexpr {
public:
    NandExpr(Nexpr* and_expr, Nexpr* equality_expr):\
        and_expr(and_expr),
        equality_expr(equality_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* and_expr;
    Nexpr* equality_expr;
};

/**
 * P == Q
 * or
 * P != Q
 */
class NequalityExpr: public Nexpr {
public:
    enum EQUALITY_OP {EQ_OP = 0, NE_OP = 1};
    NequalityExpr(Nexpr* equality_expr, EQUALITY_OP equality_op, Nexpr* relational_expr):\
        equality_expr(equality_expr),
        equality_op(equality_op),
        relational_expr(relational_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* equality_expr;
    EQUALITY_OP equality_op;
    Nexpr* relational_expr;
};

/**
 * P < Q or P > Q or P <= Q or P >= Q
 */
class NrelationalExpr: public Nexpr {
public:
    enum RELATIONAL_OP {L_OP = 0, G_OP = 1, LE_OP = 2, GE_OP = 3};
    NrelationalExpr(Nexpr* relational_expr, RELATIONAL_OP relational_op, Nexpr* shift_expr):\
        relational_expr(relational_expr),
        relational_op(relational_op),
        shift_expr(shift_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* relational_expr;
    RELATIONAL_OP relational_op;
    Nexpr* shift_expr;
};

/**
 * P << Q or P >> Q
 */
class NshiftExpr: public Nexpr {
public:
    enum SHIFT_OP {LEFT_OP = 0, RIGHT_OP = 1};
    NshiftExpr(Nexpr* shift_expr, SHIFT_OP shift_op, Nexpr* additive_expr):\
        shift_expr(shift_expr),
        shift_op(shift_op),
        additive_expr(additive_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* shift_expr;
    SHIFT_OP shift_op;
    Nexpr* additive_expr;
};

class NadditiveExpr: public Nexpr {
public:
    enum ADDITIVE_OP {PLUS_OP = 0, MINUS_OP = 1};
    NadditiveExpr(Nexpr* additive_expr, ADDITIVE_OP additive_op, Nexpr* multiplicative_expr):\
        additive_expr(additive_expr),
        additive_op(additive_op),
        multiplicative_expr(multiplicative_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* additive_expr;
    ADDITIVE_OP additive_op;
    Nexpr* multiplicative_expr;
};

/**
 * P * Q or P / Q or P % Q
 */
class NmultiplicativeExpr: public Nexpr {
public:
    enum MULTIPLICATIVE_OP {MULTIPLY_OP = 0, DIVIDE_OP = 1, MOD_OP = 2};
    NmultiplicativeExpr(Nexpr* multiplicative_expr, MULTIPLICATIVE_OP multiplicative_op, Nexpr* shift_expr):\
        multiplicative_expr(multiplicative_expr),
        multiplicative_op(multiplicative_op),
        shift_expr(shift_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    Nexpr* multiplicative_expr;
    MULTIPLICATIVE_OP multiplicative_op;
    Nexpr* shift_expr;
};

/**
 * (int)x
 */
class NcastExpr: public Nexpr {
public:
    NcastExpr(NtypeSpecifier* type_specifier, Nexpr* cast_expr):\
        type_specifier(type_specifier),
        cast_expr(cast_expr) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    NtypeSpecifier* type_specifier;
    Nexpr* cast_expr;
};

/**
 * ++x or --x or sizeof(x)
 * or
 * ~x (or ~(int)x) or sizeof(int)
 * TODO: maybe we can combine `unary_expr` and `cast_expr` into one member (or use `union`)!
 */
class NunaryExpr: public Nexpr {
public:
    enum UNARY_OP {
        INC_OP = 0, // followed by a unary_expr
        DEC_OP = 1, // followed by a unary_expr
        SIZEOF_U = 2, // followed by a unary_expr
        SIZEOF_T = 3, // followed by a type_specifier
        AND_OP = 4, // followed by a cast_expr
        MULTIPLY_OP = 5, // followed by a cast_expr
        PLUS_OP = 6, // followed by a cast_expr
        MINUS_OP = 7, // followed by a cast_expr
        TILDE_OP = 8, // followed by a cast_expr
        EXCLAMATION_OP = 9 // followed by a cast_expr
    };
    NunaryExpr(UNARY_OP unary_op, Nexpr* unary_expr, Nexpr* cast_expr):\
        unary_op(unary_op),
        unary_expr(unary_expr),
        cast_expr(cast_expr) {}
    NunaryExpr(UNARY_OP unary_op, NtypeSpecifier* type_specifier):\
        unary_op(unary_op),
        type_specifier(type_specifier) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    UNARY_OP unary_op;
    //Only one of the following should be valid!
    Nexpr* unary_expr;
    Nexpr* cast_expr;
    NtypeSpecifier* type_specifier;
};

/**
 * x[3] or f() or f(3, 4) or a.id or a->id or x++ or x--
 */
class NpostfixExpr: public Nexpr {
public:
    enum POSTFIX_TYPE {
        SQUARE_BRACKETS = 0,
        PARENTHESES = 1,
        DOT = 2,
        PTR_OP = 3,
        INC_OP = 4,
        DEC_OP = 5
    };
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr* postfix_expr, Nexpr* expr):\
        postfix_expr(postfix_expr),
        postfix_type(postfix_type),
        expr(expr) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr* postfix_expr, std::vector<Nexpr*>& argument_expr_list):\
        postfix_expr(postfix_expr),
        postfix_type(postfix_type),
        argument_expr_list(argument_expr_list) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr* postfix_expr, Nidentifier* identifier):\
        postfix_expr(postfix_expr),
        postfix_type(postfix_type),
        identifier(identifier) {}
    NpostfixExpr(POSTFIX_TYPE postfix_type, Nexpr* postfix_expr):\
        postfix_expr(postfix_expr),
        postfix_type(postfix_type) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    POSTFIX_TYPE postfix_type;
    Nexpr* postfix_expr;
    Nexpr* expr; // valid when postfix_type = SUQARE_BRACKETS
    std::vector<Nexpr*> argument_expr_list; // valid when postfix_type = PARENTHESES, could be a nullptr!
    Nidentifier* identifier; // valid when postfix_type = DOT or PTR_OP
    // when postfix_type = INC_OP or DEC_OP, none of the 3 upon would be used!
};

/**
 * x or 'c' or 3 or 3.14
 * TODO: Implement with STRING_LITERAL
 */
class NprimaryExpr: public Nexpr {
public:
    enum PRIMARY_TYPE {
        IDENTIFIER = 0,
        CONSTANT = 1
    };
    NprimaryExpr(Nidentifier* identifier):identifier(identifier), primary_type(IDENTIFIER) {}
    NprimaryExpr(Nconstant* constant):constant(constant), primary_type(CONSTANT) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    PRIMARY_TYPE primary_type; // tell whether the primary expression is an identifier or a constant
    Nidentifier* identifier;
    Nconstant* constant;
};

/**
 * `IDENTIFIER` node -- an identifier
 * @param name: a std::string variable for the identifier's name
 */
class Nidentifier: public Nexpr {
public:
    Nidentifier(std::string& name):name(name) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    std::string name;
};

/**
 * `CONSTANT` node -- a constant
 * @param type: RCC_CHAR, RCC_INT or RCC_DOUBLE
 * @param value: a RCC_CHAR, RCC_INT or RCC_DOUBLE constant value
 */
class Nconstant: public Node {
public:
    Nconstant(RCC_TYPE type, char value):type(type) { this->value.char_value = value; }
    Nconstant(RCC_TYPE type, int value):type(type) { this->value.int_value = value; }
    Nconstant(RCC_TYPE type, double value):type(type) { this->value.double_value = value; }
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    RCC_TYPE type;
    union Value {
        char char_value;
        int int_value;
        double double_value;
    } value;
};

#endif