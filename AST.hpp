#include <iostream>
#include <llvm/IR/Value.h>
#include <vector>
#include <string>

class Node {
public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenCtx &ctx) {}
};

class NExpression: public Node {};
