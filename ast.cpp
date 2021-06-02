#include "AST.hpp"
using namespace std;
extern StructAST *selfDefinedType;
static map<string, StructAST *> structBindings;
DerivedTypeAST::DerivedTypeAST(char type) : PrototypeAST("int")
{
    switch (type)
    {
    case 'c':
        op = "char";
        break;
    // case 'i':op="int";break;
    case 'd':
        op = "double";
        break;
    case 't':
        op = selfDefinedType->op;
        if (structBindings.find(op) == structBindings.end())
            error("struct not defined");
        break;
    }
#ifdef _SHOW_CONSTRUCTOR
    printf("DerivedType\n");
#endif
}
StructAST::StructAST(const std::string &op, DecAST *content)
    : PrototypeAST(op), content(content)
{
#ifdef _SHOW_CONSTRUCTOR
    printf("%s", "StructAST\n");
#endif
    if (bindings.find(op) != bindings.end() || structBindings.find(op) != structBindings.end())
        error("struct name conflict");
    structBindings[op] = this;
}
