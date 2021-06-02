#include "AST.hpp"
#include <vector>
using namespace std;
extern Nstruct *selfDefinedType;
static map<string, Nstruct *> structBindings;
void Nprogram::printNode(int indent = 0)
{
    PRINT_INDENT(indent, "Nprogram");
    for (auto &external_declaration : external_declaration_list)
    {
        external_declaration->printNode(indent + 1);
    }
}

void Ndeclaration::printNode(int indent)
{
    PRINT_INDENT(indent, "Ndeclaration");
    type_specifier->printNode(indent + 1);
    for (auto &init_declarator : init_declarator_list)
    {
        init_declarator->printNode(indent + 1);
    }
}


void NinitDeclarator::printNode(int indent)
{
    PRINT_INDENT(indent, "NinitDeclarator");
    if (initializer != nullptr)
        initializer->printNode(indent + 1);
}

void NdirectDeclarator::printNode(int indent)
{
    switch (direct_declarator_type)
    {
    case IDENTIFIER:
        PRINT_INDENT(indent, "NdirectDeclarator(IDENTIFIER)");
        identifier->printNode(indent + 1);
        break;
    case SQUARE_BRACKET_CONSTANT:
        identifier->printNode(indent + 1);
        PRINT_INDENT(indent, "NdirectDeclarator(SQUARE_BRACKET_CONSTANT, ", false);
        std::cout << "INT CONSTANT:";
        if (!int_constant_list.empty())
        {
            for (auto &int_value : int_constant_list)
            {
                std::cout << " " << int_value;
            }
        }
        std::cout << ")" << std::endl;
        break;
        break;
    case SQUARE_BRACKET_EMPTY:
        identifier->printNode(indent + 1);
        PRINT_INDENT(indent, "NdirectDeclarator(SQUARE_BRACKET_EMPTY, ", false);
        std::cout << "INT CONSTANT:";
        if (!int_constant_list.empty())
        {
            for (auto &int_value : int_constant_list)
            {
                std::cout << " " << int_value;
            }
        }
        std::cout << ")" << std::endl;
        break;
    case PARENTHESES_PARAMETER_LIST:
        identifier->printNode(indent + 1);
        PRINT_INDENT(indent, "NdirectDeclarator(PARENTHESES_PARAMETER_LIST)");
        for (auto &parameter : parameter_list)
            parameter->printNode(indent + 1);
        break;
    case PARENTHESES_IDENTIFIER_LIST:
        identifier->printNode(indent + 1);
        PRINT_INDENT(indent, "NdirectDeclarator(PARENTHESES_IDENTIFIER_LIST)");
        for (auto &identifier : identifier_list)
            identifier->printNode(indent + 1);
        break;
    case PARENTHESES_EMPTY:
        identifier->printNode(indent + 1);
        PRINT_INDENT(indent, "NdirectDeclarator(PARENTHESES_EMPTY)");
        break;
    default:
        PRINT_INDENT(indent, "NdirectDeclarator(ERROR)");
    }
    if (initializer != nullptr)
        initializer->printNode(indent + 1);
}

void Ninitializer::printNode(int indent)
{
    PRINT_INDENT(indent, "Ninitializer");
    if (assign_expr)
        assign_expr->printNode(indent + 1);
    for (auto &initializer : initializer_list)
    {
        initializer->printNode(indent + 1);
    }
}

void NtypeSpecifier::printNode(int indent)
{
    PRINT_INDENT(indent, "NtypeSpecifier(" + type + ")");
}

void NfunctionDefinition::printNode(int indent)
{
    PRINT_INDENT(indent, "NfunctionDefinition");
    if (type_specifier)
        type_specifier->printNode(indent + 1);
    if (direct_declarator)
        direct_declarator->printNode(indent + 1);
    // if(!declaration_list.empty())
    //     for(auto& declaration: declaration_list)
    //     {
    //         declaration->printNode(indent + 1);
    //     }
    if (compound_statement)
        compound_statement->printNode(indent + 1);
}

void NcompoundStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NcompoundStatement");
    for (auto &declaration : declaration_list)
    {
        declaration->printNode(indent + 1);
    }
    for (auto &statement : statement_list)
    {
        statement->printNode(indent + 1);
        // PRINT_INDENT(indent + 1, "Nstatement");
    }
}

void Nidentifier::printNode(int indent)
{
    PRINT_INDENT(indent, "Nidentifier", false);
    std::cout << "(\'" << name << "\')" << std::endl;
}

void Nconstant::printNode(int indent)
{
    if (type == "char")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(char: "
                  << "\'" << value.char_value << "\')" << std::endl;
    }
    else if (type == "int")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(int: " << value.int_value << ")" << std::endl;
    }
    else if (type == "double")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(double: " << value.double_value << ")" << std::endl;
    }
    else if (type == "string_literal")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(string_literal: \"" << value.string_literal_value << "\")" << std::endl;
    }
    else if (type == "error")
    {
        PRINT_INDENT(indent, "Nconstant(error)");
    }
    else
    {
        PRINT_INDENT(indent, "Nconstant(NULL)");
    }
}

void NparameterDeclaration::printNode(int indent)
{
    PRINT_INDENT(indent, "NparameterDeclaration");
    if (type_specifier)
        type_specifier->printNode(indent + 1);
    if (direct_declarator)
        direct_declarator->printNode(indent + 1);
}

void NexprStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NexprStatement");
    expr->printNode(indent + 1);
}

void NifStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NifStatement");
    cond_expr->printNode(indent + 1);
    if_statement->printNode(indent + 1);
    if(else_statement) else_statement->printNode(indent + 1);
}

void Nexpr::printNode(int indent)
{
    PRINT_INDENT(indent, "Nexpr");
    for (auto &expr : expr_list)
        expr->printNode(indent + 1);
}

void NassignExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NassignExpr(type: " + type + ")");
}

void NcondExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NcondExpr");
}

void NbinaryExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NbinaryExpr(op: " + op + ", type: " + type + ")");
    if (lhs)
        lhs->printNode(indent + 1);
    if (rhs)
        rhs->printNode(indent + 1);
}

void NcastExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NcastExpr");
}

void NunaryExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NunaryExpr");
}

void NpostfixExpr::printNode(int indent)
{
    // postfix_expr->printNode(indent);
    PRINT_INDENT(indent, "NpostfixExpr(" + dynamic_cast<Nidentifier *>(postfix_expr)->name + ")");
    for (auto it : argument_expr_list)
    {
        it->printNode(indent + 1);
    }
}

void Ndeclaration::bind()
{
    // Binding!
    std::string type = type_specifier->type;

    for (auto &init_declarator : init_declarator_list)
    {
        if (NdirectDeclarator *direct_declarator = dynamic_cast<NdirectDeclarator *>(init_declarator))
            direct_declarator->bind(type);
    }
}

void NdirectDeclarator::bind(std::string type)
{
    if (direct_declarator_type == IDENTIFIER)
    {
        binding_info_map[identifier->name] = type;
        std::cout << std::endl
                  << "Binding \'" << identifier->name << "\' -> \'" << type << "\'...";
    }
    else if (direct_declarator_type == SQUARE_BRACKET_CONSTANT)
    {
        for (auto &int_value : int_constant_list)
        {
            type += "|" + INT2STRING(int_value);
        }
        binding_info_map[identifier->name] = type;
    }
    else if (direct_declarator_type == SQUARE_BRACKET_EMPTY)
    {
        // Not implemented!
        std::cout << "Unexpected error!" << std::endl;
    }
    else if (direct_declarator_type == PARENTHESES_PARAMETER_LIST)
    {
        //TODO: more to implement
        type += "|function";
        binding_info_map[identifier->name] = type;
    }
    else if (direct_declarator_type == PARENTHESES_IDENTIFIER_LIST)
    {
        //TODO: more to implement
        type += "|function";
        binding_info_map[identifier->name] = type;
    }
    else if (direct_declarator_type == PARENTHESES_EMPTY)
    {
        type += "|function";
        binding_info_map[identifier->name] = type;
    }
    else
    {
        std::cout << "Unexpected error!" << std::endl;
    }
}

void NfunctionDefinition::bind()
{
    // Binding!
    std::string type = type_specifier->type;

    if (direct_declarator)
        direct_declarator->bind(type);
}

void NdirectDeclarator::pushIntConstant(Nconstant *int_constant)
{
    // add the `int_value` of `int_constant` into the `int_constant_list`
    int_constant_list.push_back(int_constant->value.int_value);
}

void NdirectDeclarator::updateType(DIRECT_DECLARATOR_TYPE direct_declarator_type)
{
    // update the type if necessary
    // TODO: check declarator's syntax here!!!
    if (this->direct_declarator_type != IDENTIFIER && this->direct_declarator_type != direct_declarator_type)
    {
        std::cout << "Latent Error: Mixed type in declarator!" << std::endl;
    }
    this->direct_declarator_type = direct_declarator_type;
}

void NdirectDeclarator::setParameterList(const std::vector<NparameterDeclaration *> &parameter_list)
{
    this->parameter_list = parameter_list;
}

void NdirectDeclarator::setIdentifierList(const std::vector<Nidentifier *> &identifier_list)
{
    this->identifier_list = identifier_list;
}
NderivedType::NderivedType(char type) 
{
    switch (type)
    {
    case 'c':
        baseType = "char";
        break;
    // case 'i':baseType="int";break;
    case 'd':
        baseType = "double";
        break;
    case 't':
        baseType = selfDefinedType->name;
        if (structBindings.find(baseType) == structBindings.end())
            error("struct not defined");
        break;
    }
}
void Nstruct::printNode(int indent)
{
    PRINT_INDENT(indent, "");
    if (content)
        for(auto it:*content)
            it->printNode(indent + 1);
}
Nstruct::Nstruct(const std::string &name,vector<Ndeclaration *> *content) : name(name), content(content)
{
    if (bindings.find(name) != bindings.end() || structBindings.find(name) != structBindings.end())
        error("struct name conflict");
    structBindings[name] = this;
}
