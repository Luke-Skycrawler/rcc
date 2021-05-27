#include <iostream>
#include <llvm/IR/Value.h>
#include <vector>
#include <string>

enum RCCTYPE {CHAR = 1, INT = 2, DOUBLE = 3};

/**
 * Base class of AST node, for derivation and inheritation
 */
class Node {
public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
};

/**
 * `declaration` node -- a declaration like 'int x = 3', consisting of
 * @param declaration_specifiers: like type_specifier (TODO: storage_type like 'static' and type qualifier like 'const' should be implemented)
 * @param init_declarator_list
 */
class Ndeclaration: public Node {
    Ndeclaration(vector<NdeclarationSpecifiers*> declaration_specifiers, vector<NinitDeclarator*> init_declarator_list):\
        declaration_specifiers(declaration_specifiers),
        init_declarator_list(init_declarator_list) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    vector<NdeclarationSpecifiers*> declaration_specifiers; // Only one member now, like 'int' in 'int x = 3'
    vector<NinitDeclarator*> init_declarator_list;
};

/**
 * `declaration_specifiers` node -- a declaration specifer looks like `static int` or `int` or `const int`
 * TODO: implement it with full feature
 */
class NdeclarationSpecifiers: public Node {
public:
    NdeclarationSpecifiers(NtypeSpecifier type):type(type) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    NtypeSpecifier* type_specifier;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 */
class NinitDeclarator: public Node {
    NinitDeclarator(Ndeclarator* declarator, Ninitializer* initializer):declarator(declarator), initializer(initializer) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
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
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    NdirectDeclarator* direct_declarator;
}

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', '(x)' or 'x[30]'
 * TODO: only a single identifier like 'x' is implemented now, more to implement later
 */
class NdirectDeclarator: public Node {
    NdirectDeclarator(Nidentifier* identifier):identifier(identifier) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    Nidentifier* identifier;
}

/**
 * 
 * TODO: a straw man implementation using CONSTANT only! Should replace it with an assign expression and more (probably an initializer list, but maybe unnecessary...)
 */
class Ninitializer: public Node {
public:
    Ninitializer(Nconstant* constant):constant(constant) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    Nconstant* constant;
}

/**
 * `compound_statement` node -- a compound statement, consisting of
 * @param statement_list: a statement list
 * @param declaration_list: a declaration list
 */
class NcompoundStatement: public Node {
public:
    NcompoundStatement(std::vector<Nstatement*>& statement_list, std::vector<Ndeclaration*> declaration_list):\
        statement_list(statement_list),
        declaration_list(declaration_list) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    std::vector<Nstatement*> statement_list;
    std::vector<Ndeclaration*> declaration_list;
};

/**
 * `statement` node -- a statement
 * TODO: implement it
 */
class Nstatement: public Node {};

/**
 * `type_specifier` node -- 'char', 'int' or 'double'
 */
class NtypeSpecifier: public NdeclarationSpecifiers {
public:
    NtypeSpecifier(RCCTYPE type):type(type) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    RCCTYPE type;
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
    Nidentifier(const std::string& name):name(name) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    std::string name;
};

/**
 * `CONSTANT` node -- a constant
 * @param type: CHAR, INT or DOUBLE
 * @param value: a CHAR, INT or DOUBLE constant value
 */
class Nconstant: public Nexpr {
public:
    union Value {
        char char_value;
        int int_value;
        double double_value;
    };
    Nconstant(RCCTYPE type, char value):type(type), value(value.char_value) {}
    Nconstant(RCCTYPE type, int value):type(type), value(value.int_value) {}
    Nconstant(RCCTYPE type, double value):type(type), value(value.double_value) {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx);
private:
    RCCTYPE type;
    Value value;
};