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
    PRINT_INDENT(indent, "NtypeSpecifier(" + type + ")");
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
    if(type == "char")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(char: " << "\'" << value.char_value << "\')" << std::endl;
    }
    else if(type == "int")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(int: " << value.int_value << ")" << std::endl;
    }
    else if(type == "double")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(double: " << value.double_value << ")" << std::endl;
    }
    else if(type == "string_literal")
    {
        PRINT_INDENT(indent, "Nconstant", false);
        std::cout << "(string_literal: \"" << value.string_literal_value << "\")" << std::endl;
    }
    else if(type == "error")
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
    PRINT_INDENT(indent, "NassignExpr(type: " + type + ")");
}

void NcondExpr::printNode(int indent)
{
    PRINT_INDENT(indent, "NcondExpr");
}

void NbinaryExpr::printNode(int indent){
    PRINT_INDENT(indent, "NbinaryExpr(op: " + op + ", type: " + type + ")");
    if(lhs) lhs->printNode(indent + 1);
    if(rhs) rhs->printNode(indent + 1);
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

void Ndeclaration::bind()
{
    // Binding!
    std::string type = declaration_specifiers->type_specifier->type;

    for(auto& init_declarator: init_declarator_list)
    {
        if(NdirectDeclarator* direct_declarator = dynamic_cast<NdirectDeclarator*>(init_declarator->declarator))
            direct_declarator->bind(type, "");
    }
}

void NdirectDeclarator::bind(std::string type, std::string additional_info)
{
    if(direct_declarator_type == IDENTIFIER)
    {
        binding_info_map[identifier->name] = type + additional_info;
        std::cout << std::endl << "Binding \'" << identifier->name << "\' -> \'" << type + additional_info << "\'...";
    }
    else if(direct_declarator_type == NESTED_DECLARATOR)
    {
        if(direct_declarator)
            direct_declarator->bind(type, additional_info);
    }
    else if(direct_declarator_type == SQUARE_BRACKET_CONSTANT)
    {
        std::string tmp(INT2STRING(int_constant->value.int_value));
        tmp = tmp + additional_info; // appending '[INT_CONSTANT]' in the front
        tmp = "|" + tmp;
        if(direct_declarator)
            direct_declarator->bind(type, tmp);
    }
    else if(direct_declarator_type == SQUARE_BRACKET_EMPTY)
    {
        // Not implemented!
        std::cout << "Unexpected error!" << std::endl;
    }
    else if(direct_declarator_type == PARENTHESES_PARAMETER_LIST)
    {
        //TODO: more to implement
        additional_info += "|function";
        if(direct_declarator)
            direct_declarator->bind(type, additional_info);
    }
    else if(direct_declarator_type == PARENTHESES_IDENTIFIER_LIST)
    {
        //TODO: more to implement
        additional_info += "|function";
        if(direct_declarator)
            direct_declarator->bind(type, additional_info);
    }
    else if(direct_declarator_type == PARENTHESES_EMPTY)
    {
        additional_info += "|function";
        if(direct_declarator)
            direct_declarator->bind(type, additional_info);
    }
    else
    {
        std::cout << "Unexpected error!" << std::endl;
    }
}

void NfunctionDefinition::bind()
{
    // Binding!
    std::string type = declaration_specifiers->type_specifier->type;

    if(NdirectDeclarator* direct_declarator = declarator->direct_declarator)
        direct_declarator->bind(type, "");
}