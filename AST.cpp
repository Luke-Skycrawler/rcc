#include "AST.hpp"

void Nprogram::printNode(int indent = 0)
{
    PRINT_INDENT(indent, "Nprogram");
    for(auto&external_declaration: external_declaration_list)
    {
        external_declaration->printNode(indent + 1);
    }
}

void Ndeclaration::printNode(int indent)
{
    PRINT_INDENT(indent, "Ndeclaration");
    declaration_specifiers->printNode(indent + 1);
    for(auto& init_declarator: init_declarator_list)
    {
        init_declarator->printNode(indent + 1);
    }
}

void NdeclarationSpecifiers::printNode(int indent)
{
    PRINT_INDENT(indent, "NdeclarationSpecifiers");
    type_specifier->printNode(indent + 1);
}

void NinitDeclarator::printNode(int indent)
{
    PRINT_INDENT(indent, "NinitDeclarator");
    declarator->printNode(indent + 1);
    if(initializer != nullptr)
        initializer->printNode(indent + 1);
}

// void Ndeclarator::printNode(int indent)
// {
//     PRINT_INDENT(indent, "Ndeclarator");
//     direct_declarator->printNode(indent + 1);
// }

void NdirectDeclarator::printNode(int indent)
{
    PRINT_INDENT(indent, "NdirectDeclarator");
    switch(direct_declarator_type)
    {
        case IDENTIFIER:
            identifier->printNode(indent + 1);
            break;
        case NESTED_DECLARATOR:
            declarator->printNode(indent + 1);
            break;
        case SQUARE_BRACKET_CONSTANT:
            direct_declarator->printNode(indent + 1);
            int_constant->printNode(indent + 1);
            break;
        case SQUARE_BRACKET_EMPTY:
            direct_declarator->printNode(indent + 1);
            break;
        case PARENTHESES_PARAMETER_LIST:
            direct_declarator->printNode(indent + 1);
            for(auto& parameter: parameter_list)
                parameter->printNode(indent + 1);
            break;
        case PARENTHESES_IDENTIFIER_LIST:
            direct_declarator->printNode(indent + 1);
            for(auto& identifier: identifier_list)
                identifier->printNode(indent + 1);
            break;
        case PARENTHESES_EMPTY:
            direct_declarator->printNode(indent + 1);
            break;
        default:
            std::cout << "NONE" << std::endl;
    }
}

void Ninitializer::printNode(int indent)
{
    PRINT_INDENT(indent, "Ninitializer");
    if(assign_expr) assign_expr->printNode(indent + 1);
    for(auto& initializer: initializer_list)
    {
        initializer->printNode(indent + 1);
    }
}

void NtypeSpecifier::printNode(int indent)
{
    switch(type)
    {
        case RCC_CHAR:
            PRINT_INDENT(indent, "NtypeSpecifier(char)");
            break;
        case RCC_INT:
            PRINT_INDENT(indent, "NtypeSpecifier(int)");
            break;
        case RCC_DOUBLE:
            PRINT_INDENT(indent, "NtypeSpecifier(double)");
            break;
        default:
            PRINT_INDENT(indent, "NtypeSpecifier(NULL)");
            break;
    }
}

void NfunctionDefinition::printNode(int indent)
{
    PRINT_INDENT(indent, "NfunctionDefinition");
    if(declaration_specifiers) declaration_specifiers->printNode(indent + 1);
    if(declarator) declarator->printNode(indent + 1);
    // if(!declaration_list.empty())
    //     for(auto& declaration: declaration_list)
    //     {
    //         declaration->printNode(indent + 1);
    //     }
    if(compound_statement) compound_statement->printNode(indent + 1);
}

void NcompoundStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NcompoundStatement");
    for(auto& declaration: declaration_list)
    {
        declaration->printNode(indent + 1);
    }
    for(auto& statement: statement_list)
    {
        statement->printNode(indent + 1);
        // PRINT_INDENT(indent + 1, "Nstatement");
    }
}

void Nidentifier::printNode(int indent)
{
    PRINT_INDENT(indent, "Nidentifier", false);
    std::cout << "(" << name << ")" << std::endl;
}

void Nconstant::printNode(int indent)
{
    switch(type)
    {
        case RCC_CHAR:
            PRINT_INDENT(indent, "Nconstant", false);
            std::cout << "(char: " << "\'" << value.char_value << "\')" << std::endl;
            break;
        case RCC_INT:
            PRINT_INDENT(indent, "Nconstant", false);
            std::cout << "(int: " << value.int_value << ")" << std::endl;
            break;
        case RCC_DOUBLE:
            PRINT_INDENT(indent, "Nconstant", false);
            std::cout << "(double: " << value.double_value << ")" << std::endl;
            break;
        case RCC_STRING_LITERAL:
            PRINT_INDENT(indent, "Nconstant", false);
            std::cout << "(string literal: \"" << value.string_literal_value << "\")" << std::endl;
            break;
    }
    
}

void NparameterDeclaration::printNode(int indent)
{
    PRINT_INDENT(indent, "NparameterDeclaration");
    if(declaration_specifiers) declaration_specifiers->printNode(indent + 1);
    if(declarator) declarator->printNode(indent + 1);
}

void NexprStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NexprStatement");
    expr->printNode(indent + 1);
}

void Nexpr::printNode(int indent)
{
    PRINT_INDENT(indent, "Nexpr");
    for(auto& expr: expr_list)
        expr->printNode(indent + 1);
}

void NassignExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NassignExpr");
}

void NcondExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NcondExpr");
}
void NbinaryExpr::printNode(int indent){
    PRINT_INDENT(indent,"NbianryExpr("+op+")");
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
    PRINT_INDENT(indent, "NpostfixExpr");
}

