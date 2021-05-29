#ifndef _AST_HPP
#define _AST_HPP
#include <iostream>
#include "CodeGen.h"
#include <vector>
#include <string>

#ifdef DEBUG
#define PRINT_NODE(s)  std::cout << std::endl << "#" << s << " node created" << std::endl 
#endif

#ifndef DEBUG
#define PRINT_NODE(s)
#endif

enum RCC_TYPE {RCC_CHAR = 1, RCC_INT = 2, RCC_DOUBLE = 3};

class Node;
class Nprogram;
class NexternalDeclaration;
class Ndeclaration;
class NdeclarationSpecifiers;
class NinitDeclarator;
class Ndeclarator;
class NdirectDeclarator;
class Ninitializer;
class NfunctionDefinition;
class NcompoundStatement;
class Nstatement;
class NexprStatement;
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
    virtual llvm::Value* codeGen()=0;
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
private:
    std::vector<NexternalDeclaration*> external_declaration_list;
};

/**
 * `external_declaration` node -- either a declaration or a function-definition
 * It's a base virtual node for
 * - `Ndeclaration`
 * - `NfunctionDefinition`
 */
class NexternalDeclaration: public Node {};

/**
 * `declaration` node -- a declaration looks like 'int x = 3', consisting of
 * @param declaration_specifiers: like type_specifier (TODO: storage_type like 'static' and type qualifier like 'const' should be implemented)
 * @param init_declarator_list
 */
class Ndeclaration: public NexternalDeclaration {
public:
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers, std::vector<NinitDeclarator*>& init_declarator_list):\
        declaration_specifiers(declaration_specifiers),
        init_declarator_list(init_declarator_list) {PRINT_NODE("Ndeclaration");}
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers):\
        declaration_specifiers(declaration_specifiers) {PRINT_NODE("Ndeclaration");}
    llvm::Value* codeGen();
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
    NdeclarationSpecifiers(NtypeSpecifier* type_specifier):type_specifier(type_specifier) {PRINT_NODE("NdeclarationSpecifiers");}
    llvm::Value* codeGen();
private:
    NtypeSpecifier* type_specifier;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 */
class NinitDeclarator: public Node {
public:
    NinitDeclarator(Ndeclarator* declarator, Ninitializer* initializer):declarator(declarator), initializer(initializer) {PRINT_NODE("NinitDeclarator");}
    llvm::Value* codeGen();
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
    Ndeclarator(NdirectDeclarator* direct_declarator):direct_declarator(direct_declarator) {PRINT_NODE("Ndeclarator");}
    llvm::Value* codeGen();
private:
    NdirectDeclarator* direct_declarator;
};

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', '(x)' or 'x[30]'
 * TODO: only a single identifier like 'x' is implemented now, more to implement later
 */
class NdirectDeclarator: public Node {
public:
    NdirectDeclarator(Nidentifier* identifier):identifier(identifier) {PRINT_NODE("NdirectDeclarator");}
    llvm::Value* codeGen();
private:
    Nidentifier* identifier;
};

/**
 * 
 * TODO: a straw man implementation using CONSTANT only! Should replace it with an assign expression and more (probably an initializer list, but maybe unnecessary...)
 */
class Ninitializer: public Node {
public:
    Ninitializer(Nconstant* constant):constant(constant) {PRINT_NODE("Ninitializer");}
    llvm::Value* codeGen();
private:
    Nconstant* constant;
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
        declaration_list(declaration_list) {PRINT_NODE("NcompoundStatement");}
    NcompoundStatement(const std::vector<Ndeclaration*>& declaration_list):declaration_list(declaration_list) {PRINT_NODE("NcompoundStatement");}
    NcompoundStatement() {PRINT_NODE("NcompoundStatement");}
    llvm::Value* codeGen();
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
    llvm::Value* codeGen();
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
    llvm::Value* codeGen();
private:
    RCC_TYPE type;
    union Value {
        char char_value;
        int int_value;
        double double_value;
    } value;
};

#endif