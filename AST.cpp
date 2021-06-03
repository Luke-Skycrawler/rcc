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
    if(op==""){
        PRINT_INDENT(indent, "NdirectDeclarator(IDENTIFIER)");
        identifier->printNode(indent + 1);
        return;
    } 
    switch (op[0])
    {
        case '[':
            identifier->printNode(indent + 1);
            PRINT_INDENT(indent, "NdirectDeclarator(SQUARE_BRACKET_CONSTANT, ", false);
            std::cout << "INT CONSTANT:";
            if (!dimensions.empty())
            {
                for (auto it : dimensions)
                {
                    // std::cout << " " << int_value;
                    if(it)it->printNode(indent+1);
                    else PRINT_INDENT(indent+1,"[]");
                }
            }
            std::cout << ")" << std::endl;
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
    PRINT_INDENT(indent, "NpostfixExpr(" + name->name + " type: "+type+")");
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
    printf("Warning: Deprecated NdirecDeclarator::bind()!\n");
    if (op=="")
    {
        binding_info_map[identifier->name] = type;
        std::cout << std::endl
                  << "Binding \'" << identifier->name << "\' -> \'" << type << "\'...";
    }
    else if (op=="[]")
    {
        for (auto int_value : dimensions)
        {
            type += "|" + INT2STRING(int_value->value.int_value);
        }
        binding_info_map[identifier->name] = type;
    }
    else if (op=="()")
    {
        //TODO: more to implement
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
    // add the `int_value` of `int_constant` into the `dimensions`
    dimensions.push_back(int_constant);
}

void NdirectDeclarator::updateType(const string &op)
{
    // update the type if necessary
    // TODO: check declarator's syntax here!!!
    if (this->op != "" && this->op != op)
        error("Latent Error: Mixed type in declarator!");
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
        for (auto it : *content)
            it->printNode(indent + 1);
}
Nstruct::Nstruct(const std::string &name, vector<Ndeclaration *> *content) : name(name), content(content)
{
    if (bindings.find(name) != bindings.end() || structBindings.find(name) != structBindings.end())
        error("struct name conflict");
    structBindings[name] = this;
}
Nconstant::Nconstant(const std::string &type, char *value) 
{
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