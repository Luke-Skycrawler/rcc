#ifndef _AST_HPP
#define _AST_HPP
#include <iostream>
// #include <llvm/IR/Value.h>
#include <vector>
#include <string>

enum RCC_TYPE {RCC_CHAR = 1, RCC_INT = 2, RCC_DOUBLE = 3};

class Node;
class Ndeclaration;
class NdeclarationSpecifiers;
class NinitDeclarator;
class Ndeclarator;
class NdirectDeclarator;
class Ninitializer;
class NcompoundStatement;
class Nstatement;
class NtypeSpecifier;
class Nexpr;
class Nidentifier;
class Nconstant;

/**
 * Base class of AST node, for derivation and inheritation
 */
class Node {
public:
    virtual ~Node() {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
};

/**
 * `declaration` node -- a declaration like 'int x = 3', consisting of
 * @param declaration_specifiers: like type_specifier (TODO: storage_type like 'static' and type qualifier like 'const' should be implemented)
 * @param init_declarator_list
 */
class Ndeclaration: public Node {
public:
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers, std::vector<NinitDeclarator*>* init_declarator_list):\
        declaration_specifiers(declaration_specifiers),
        init_declarator_list(init_declarator_list) {}
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers):\
        declaration_specifiers(declaration_specifiers) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    NdeclarationSpecifiers* declaration_specifiers; // like 'int' in 'int x = 3'
    std::vector<NinitDeclarator*>* init_declarator_list;
};

/**
 * `declaration_specifiers` node -- a declaration specifer looks like `static int` or `int` or `const int`
 * TODO: implement it with full feature
 */
class NdeclarationSpecifiers: public Node {
public:
    NdeclarationSpecifiers(NtypeSpecifier* type_specifier):type_specifier(type_specifier) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    NtypeSpecifier* type_specifier;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 */
class NinitDeclarator: public Node {
public:
    NinitDeclarator(Ndeclarator* declarator, Ninitializer* initializer):declarator(declarator), initializer(initializer) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
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
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    NdirectDeclarator* direct_declarator;
};

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', '(x)' or 'x[30]'
 * TODO: only a single identifier like 'x' is implemented now, more to implement later
 */
class NdirectDeclarator: public Node {
public:
    NdirectDeclarator(Nidentifier* identifier):identifier(identifier) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    Nidentifier* identifier;
};

/**
 * 
 * TODO: a straw man implementation using CONSTANT only! Should replace it with an assign expression and more (probably an initializer list, but maybe unnecessary...)
 */
class Ninitializer: public Node {
public:
    Ninitializer(Nconstant* constant):constant(constant) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    Nconstant* constant;
};

/**
 * `compound_statement` node -- a compound statement, consisting of
 * @param statement_list: a statement list
 * @param declaration_list: a declaration list
 */
class NcompoundStatement: public Node {
public:
    NcompoundStatement(std::vector<Ndeclaration*>* declaration_list, std::vector<Nstatement*>* statement_list):\
        statement_list(statement_list),
        declaration_list(declaration_list) {}
    NcompoundStatement(std::vector<Ndeclaration*>* declaration_list):declaration_list(declaration_list) {}
    NcompoundStatement() {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    std::vector<Ndeclaration*>* declaration_list;
    std::vector<Nstatement*>* statement_list;
};

/**
 * `statement` node -- a statement
 * TODO: implement it
 */
class Nstatement: public Node {};

/**
 * `type_specifier` node -- 'char', 'int' or 'double'
 */
class NtypeSpecifier: public Node {
public:
    NtypeSpecifier(RCC_TYPE type):type(type) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    RCC_TYPE type;
};

/**
 * `expr` node -- an expression
 * The node should have no content, only to be inherited by different types of expression classes.
 */
class Nexpr: public Node {};

/**
 * `IDENTIFIER` node -- an identifier
 * @param name: a std::string variable for the identifier's name
 */
class Nidentifier: public Nexpr {
public:
    Nidentifier(std::string* name):name(name) {}
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    std::string* name;
};

/**
 * `CONSTANT` node -- a constant
 * @param type: RCC_CHAR, RCC_INT or RCC_DOUBLE
 * @param value: a RCC_CHAR, RCC_INT or RCC_DOUBLE constant value
 */
class Nconstant: public Nexpr {
public:
    Nconstant(RCC_TYPE type, char value):type(type) { this->value.char_value = value; }
    Nconstant(RCC_TYPE type, int value):type(type) { this->value.int_value = value; }
    Nconstant(RCC_TYPE type, double value):type(type) { this->value.double_value = value; }
    // virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    RCC_TYPE type;
    union Value {
        char char_value;
        int int_value;
        double double_value;
    } value;
};

#endif