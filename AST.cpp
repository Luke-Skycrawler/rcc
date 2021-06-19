#include "AST.hpp"
#include <vector>
using namespace std;
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
    if(op==""){
        PRINT_INDENT(indent, "NdirectDeclarator(IDENTIFIER)");
        identifier->printNode(indent + 1);
        return;
    } 
    switch (op[0])
    {
        case '[':
            identifier->printNode(indent + 1);
            PRINT_INDENT(indent, "NdirectDeclarator(SQUARE_BRACKET_CONSTANT)");
            if (!dimensions.empty())
            {
                for (auto it : dimensions)
                {
                    // std::cout << " " << int_value;
                    if(it)it->printNode(indent+1);
                    else PRINT_INDENT(indent+1,"[]");
                }
            }
            break;
        case '(':
            identifier->printNode(indent + 1);
            PRINT_INDENT(indent, "NdirectDeclarator(PARENTHESES_PARAMETER_LIST)");
            for (auto &parameter : parameter_list)
                parameter->printNode(indent + 1);
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
    if(struct_type) struct_type->printNode(indent + 1);
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
    if (else_statement)
        else_statement->printNode(indent + 1);
}

void NforStatement::printNode(int indent)
{
    if(inc) PRINT_INDENT(indent, "NforStatement(increment)");
    else PRINT_INDENT(indent, "NforStatement(decrement)");
    identifier->printNode(indent + 1);
    start_expr->printNode(indent + 1);
    end_expr->printNode(indent + 1);
    statement->printNode(indent + 1);
}

void NwhileStatement::printNode(int indent)
{
    PRINT_INDENT(indent, "NwhileStatement");
    cond_expr->printNode(indent + 1);
    statement->printNode(indent + 1);
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
    lhs->printNode(indent + 1);
    assign_expr->printNode(indent + 1);
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
    PRINT_INDENT(indent, "NpostfixExpr(\'" + name->name + "\', type: " + type + ")");
    for (auto it : argument_expr_list)
    {
        it->printNode(indent + 1);
    }
    for (auto it : expr)
    {
        it->printNode(indent + 1);
    }
}

void NdirectDeclarator::pushIntConstant(Nconstant *int_constant)
{
    // add the `int_value` of `int_constant` into the `dimensions`
    dimensions.push_back(int_constant);
}

void NdirectDeclarator::updateType(const string &op)
{
    // update the type if necessary
    // TODO: check declarator's syntax here!!!
    if (this->op != "" && this->op != op)
    {
        error("(latent) mixed type in declarator");
        exit(-1);
    }
    this->op = op;
}

void NdirectDeclarator::setParameterList(const std::vector<NparameterDeclaration *> &parameter_list)
{
    this->parameter_list = parameter_list;
}

void NdirectDeclarator::setIdentifierList(const std::vector<Nidentifier *> &identifier_list)
{
    this->identifier_list = identifier_list;
}
void Nstruct::printNode(int indent)
{
    PRINT_INDENT(indent, "Nstruct(\'" + name + "\')");
    if (content)
        for (auto it : *content)
            it->printNode(indent + 1);
}
Nconstant::Nconstant(const std::string &type, char *value)
{
    is_constant = true;
    //Ignore the last \", so use `len` but not `len + 1`
    this->type=type;
    int len = strlen(value);
    this->value.string_literal_value = (char *)malloc(len);
    memset(this->value.string_literal_value, '\0', len);
    strncpy(this->value.string_literal_value, value, len - 1);
    char *str = this->value.string_literal_value;
    int j = 0;
    for (int i = 0; i < len && i + j < len; i++)
    {
        if (str[i + j] == '\\')
        {
            if (j + i < len - 1)
                j++;
            else
                printf("error: string end with dangling '\\'\n");
            if (str[i + j] >= '0' && str[i + j] <= '9')
            {
                int tmp;
                sscanf(str + i + j, "%d", &tmp);
                if (tmp < 256)
                    str[i] = tmp;
                else
                    printf("error: invalid ASCII\n");
            }
            else
                switch (str[i + j])
                {
                case 'n':
                    str[i] = '\n';
                    break;
                case 't':
                    str[i] = '\t';
                    break;
                case '\\':
                    str[i] = '\\';
                    break;
                default:
                    printf("error: char not recongnized\n");
                }
        }
        else
            str[i] = str[i + j];
    }
}
void Ndeclaration::constructStruct(std::string struct_name, std::vector<llvm::Type*> &members)
{
    void *ret;
    auto type = type_specifier->type;
    auto llvm_type = STRING_TO_TYPE(type);
    for (auto iterator : init_declarator_list)
    {
        NdirectDeclarator *it = dynamic_cast<NdirectDeclarator *>(iterator);
        auto op = it->identifier->name;
        if (it->op == "") // A single identifier
        {
            if(llvm_type == NULL)
            {
                ERROR("type \'" + type + "\' not defined", 1);
            }
            members.push_back(llvm_type); // record in the vector
            int len = struct_info_bindings[struct_name]->len; // get length
            struct_info_bindings[struct_name]->name_offset_map[op] = len; // offset
            struct_info_bindings[struct_name]->len = len + 1; // length++
            struct_info_bindings[struct_name]->name_type_map[op] = type; // type name
            struct_info_bindings[struct_name]->name_llvmtype_map[op] = llvm_type; // llvm type
        }
        else if (it->op[0] == '[') // An array
        {
            ERROR("array inside a struct is on the way", -1);

            llvm::Type* tmp_type = STRING_TO_TYPE(type);
            llvm::ArrayType* array_type;
            for(auto constant = it->dimensions.rbegin(); constant != it->dimensions.rend(); constant++)
            {
                if (*constant)
                {
                    array_type = llvm::ArrayType::get(tmp_type, (*constant)->value.int_value);
                    tmp_type = array_type;
                }
            }

            members.push_back(array_type); // record in the vector
            int len = struct_info_bindings[struct_name]->len; // get length
            struct_info_bindings[struct_name]->name_offset_map[op] = len; // offset
            struct_info_bindings[struct_name]->len = len + 1; // length++
            struct_info_bindings[struct_name]->name_type_map[op] = type; // type name
            struct_info_bindings[struct_name]->name_llvmtype_map[op] = array_type; // llvm type
        }
        else if (it->op[0] == '(') // A function definition
        {
            ERROR("function definition inside a struct is not allowed", -1);
        }
    }
}