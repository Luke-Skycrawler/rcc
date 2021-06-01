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
    if(new_line) std::cout << msg << std::endl;
    else std::cout << msg;
}

// enum RCC_TYPE {RCC_CHAR = 1, RCC_INT = 2, RCC_DOUBLE = 3, RCC_STRING_LITERAL = 4, RCC_ERROR = -1, RCC_NULL = 0};

class Node;
class Nprogram;
class NexternalDeclaration;
class Ndeclaration;
class NdeclarationSpecifiers;
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
        init_declarator_list(init_declarator_list) { bind(); }
    Ndeclaration(NdeclarationSpecifiers* declaration_specifiers):\
        declaration_specifiers(declaration_specifiers) { bind(); }
    llvm::Value* codeGen();
    virtual void printNode(int indent);
    void bind();
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
// private:
    NtypeSpecifier* type_specifier;
};

/**
 * `init_declarator` node -- a init declarator looks like 'x = 3' or 'x'
 */
class NinitDeclarator: public Node {
public:
    NinitDeclarator(NinitDeclarator* declarator=NULL, Ninitializer* initializer=NULL):declarator(declarator), initializer(initializer) {}
    llvm::Value* codeGen();
    virtual void printNode(int indent);
    Ninitializer* initializer;
    NinitDeclarator* declarator;
};

/**
 * `declarator` node -- a declarator looks like 'x', '(x)', 'x[30]' or '*x'
 * TODO: implement the pointer feature like '*x'
 */
// class Ndeclarator: public NinitDeclarator {
// public:
//     Ndeclarator(NdirectDeclarator* direct_declarator):direct_declarator(direct_declarator) {}
//     llvm::Value* codeGen();
//     virtual void printNode(int indent);
// private:
//     NdirectDeclarator* direct_declarator;
// };

/**
 * `direct_declarator` node -- a direct declarator looks like 'x', '(x)' or 'x[30]'
 * TODO: only a single identifier like 'x' is implemented now, more to implement later
 */
class NdirectDeclarator: public NinitDeclarator {
public:
    enum DIRECT_DECLARATOR_TYPE {
        IDENTIFIER = 0,
        NESTED_DECLARATOR = 1,
        SQUARE_BRACKET_CONSTANT = 2,
        SQUARE_BRACKET_EMPTY = 3, // probably not used
        PARENTHESES_PARAMETER_LIST = 4,
        PARENTHESES_IDENTIFIER_LIST = 5, // probably not used
        PARENTHESES_EMPTY = 6
    };
    NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, Nidentifier* identifier):\
        direct_declarator_type(direct_declarator_type),
        identifier(identifier) {}
    // NdirectDeclarator(DIRECT_DECLARATOR_TYPE direct_declarator_type, NdirectDeclarator* declarator):\
    //     direct_declarator_type(direct_declarator_type),
    //     declarator(declarator) {}
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
    void bind(std::string type, std::string additional_info);
// private:
    DIRECT_DECLARATOR_TYPE direct_declarator_type;
    Nidentifier* identifier;
    // Ndeclarator* declarator;
    Nconstant* int_constant;
    NdirectDeclarator* direct_declarator;
    std::vector<NparameterDeclaration*> parameter_list;
    std::vector<Nidentifier*> identifier_list;
};

class NparameterDeclaration: public Node {
public:
    NparameterDeclaration(NdeclarationSpecifiers* declaration_specifiers, NdirectDeclarator* declarator):
        declaration_specifiers(declaration_specifiers),
        declarator(declarator) {}
    NparameterDeclaration(NdeclarationSpecifiers* declaration_specifiers):
        declaration_specifiers(declaration_specifiers) {}
    llvm::Value* codeGen();
    void printNode(int indent);
private:
    NdeclarationSpecifiers* declaration_specifiers;
    NdirectDeclarator* declarator;
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
    NfunctionDefinition(NdeclarationSpecifiers* declaration_specifiers, NdirectDeclarator* declarator, std::vector<Ndeclaration*>& declaration_list, NcompoundStatement* compound_statement):\
        declaration_specifiers(declaration_specifiers),
        declarator(declarator),
        declaration_list(declaration_list),
        compound_statement(compound_statement) { bind();}
    NfunctionDefinition(NdeclarationSpecifiers* declaration_specifiers, NdirectDeclarator* declarator, NcompoundStatement* compound_statement):\
        declaration_specifiers(declaration_specifiers),
        declarator(declarator),
        compound_statement(compound_statement) {bind();}
    NfunctionDefinition(NdirectDeclarator* declarator, std::vector<Ndeclaration*>& declaration_list, NcompoundStatement* compound_statement):\
        declarator(declarator),
        declaration_list(declaration_list),
        compound_statement(compound_statement) {bind();}
    NfunctionDefinition(NdirectDeclarator* declarator, NcompoundStatement* compound_statement):\
        declarator(declarator),
        compound_statement(compound_statement) { bind(); }
    llvm::Value* codeGen();
    void printNode(int indent);
    void bind();
// private:
    NdeclarationSpecifiers* declaration_specifiers; // 'int'
    NdirectDeclarator* declarator; // 'f(int x, double y, char z)'
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
 * @param type: a std::string "char", "int" or "double"
 */
class NtypeSpecifier: public Node {
public:
    NtypeSpecifier(std::string type):type(type) {}
    llvm::Value* codeGen();
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
class Nexpr: public Node {
public:
    Nexpr() {} // default constructor
    Nexpr(std::vector<Nexpr*>& expr_list):expr_list(expr_list) {}
    void push_back(Nexpr* expr) {
        expr_list.push_back(expr);
    }
    void printNode(int indent);
    llvm::Value* codeGen()=0;
    virtual std::string getType() {return "NULL";}
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
        assign_expr(assign_expr)    
    {
        // Calculate and check type
        if(unary_expr->getType() == "int")
        {
            if(assign_expr->getType() != "int")
            {
                type = "error";
                std::cout << "Type error!" << std::endl;
            }
            else
            {
                type = "int";
            }
        }
        else if(unary_expr->getType() == "double")
        {
            if(assign_expr->getType() != "int" && assign_expr->getType() != "double")
            {
                type = "error";
                std::cout << "Type error!" << std::endl;
            }
            else
            {
                type = "double";
            }
        }
        else if(unary_expr->getType() == "char")
        {
            if(assign_expr->getType() != "char")
            {
                type = "error";
                std::cout << "Type error!" << std::endl;
            }
            else
            {
                type = "char";
            }
        }
        else
        {
            //TODO: more to implement (like array assignment)
            type = "NULL";
        }
    }
    llvm::Value* codeGen();
    void printNode(int indent);
    std::string type;
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
 * P op Q
 */
class NbinaryExpr: public Nexpr{
public:
    NbinaryExpr(const std::string &op, Nexpr *lhs, Nexpr *rhs):op(op), lhs(lhs), rhs(rhs)
    {
        // Calculate and check type
        if(op == "||" || op == "&&" || op == "|" || op == "^" || \
        op == "&" || op == "==" || op == "!=" || op == "<" || \
        op == ">" || op == ">=" || op == "<=" || op == "<<" || \
        op == ">>"|| op == "%")
        {
            if(lhs->getType() != "int" || rhs->getType() != "int")
            {
                if(type_error_alarm)
                {
                    std::cout << "Type error!" << std::endl;
                    type_error_alarm = false;
                }
                type = "error";
            }
            else
            {
                type = "int";
            }
        }
        else if(op == "+" || op == "-" || op == "*" || op == "/" )
        {
            if((lhs->getType() != "int" && lhs->getType() != "double") || (rhs->getType() != "int" && rhs->getType() != "double"))
            {
                    if(type_error_alarm)
                    {
                        std::cout << "Type error!" << std::endl;
                        type_error_alarm = false;
                    }
                    type = "error";
            }
            else
            {
                if(lhs->getType() == "double" || rhs->getType() == "double")
                    type = "double";
                else type = "int";
            }
        }    
        else
        {
            type = "NULL";
        }
    }
    std::string op;
    llvm::Value *codeGen();
    void printNode(int indent);
    std::string getType()
    {
        return type;
    }
private:
    std::string type;
    Nexpr *lhs, *rhs;
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
 * `IDENTIFIER` node -- an identifier
 * @param name: a std::string variable for the identifier's name
 */
class Nidentifier: public Nexpr {
public:
    Nidentifier(std::string& name):name(name) {}
    llvm::Value* codeGen();
    void printNode(int indent);
    std::string getType()
    {
        std::string type = binding_info_map[name];
        return type;
    }
// private:
    std::string name;
};

/**
 * `CONSTANT` node -- a constant
 * @param type: a std::string "char", "int" or "double"
 */
class Nconstant: public Nexpr {
public:
    Nconstant(std::string type, char value):type(type) { this->value.char_value = value; }
    Nconstant(std::string type, int value):type(type) { this->value.int_value = value; }
    Nconstant(std::string type, double value):type(type) { this->value.double_value = value; }
    Nconstant(std::string type, char* value):type(type) {
        //Ignore the last \", so use `len` but not `len + 1`
        int len = strlen(value);
        this->value.string_literal_value = (char*)malloc(len);
        memset(this->value.string_literal_value, '\0', len);
        strncpy(this->value.string_literal_value, value, len - 1);
        char *str=this->value.string_literal_value;
        int j=0;
        for(int i=0;i<len && i+j<len;i++){
            if(str[i]=='\\'){
                if(j+i<len-1)j++;
                else printf("error: string end with dangling '\\'\n");
                if(str[i+j]>='0' && str[i+j]<='9'){
                    int tmp;
                    sscanf(str+i+j,"%d",&tmp);
                    if(tmp<256)str[i]=tmp;
                    else printf("error: invalid ASCII\n");
                }
                else switch(str[i+j]){
                    case 'n':
                        str[i]='\n';break;
                    case 't':
                        str[i]='\t';break;
                    case '\\':
                        str[i]='\\';break;
                    default:
                        printf("error: char not recongnized\n");
                }
            }
            else str[i]=str[i+j];
        }
    }
    llvm::Value* codeGen();
    void printNode(int indent);
    std::string getType()
    {
        return type;
    }
// private:
    std::string type;
    union Value {
        char char_value;
        int int_value;
        double double_value;
        char* string_literal_value;
    } value;
};

#endif