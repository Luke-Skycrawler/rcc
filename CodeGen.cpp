#include <vector>
#include "AST.hpp"
using namespace std;
using namespace llvm;
Value *Nstatement::codeGen()
{
    // auto tmp = expr->codeGen();
    // if (next)
    //     next->codeGen();
    // return tmp;
    return NULL;
}
Value *NbinaryExpr::codeGen()
{
    Value *l = lhs->codeGen(), *r = rhs->codeGen(), *ret = NULL;
    bool double_flag;
    std::string lhs_type = lhs->type;
    std::string rhs_type = rhs->type;
    //Check for type error
    if (lhs_type != "int" && lhs_type != "double" || rhs_type != "int" && rhs_type != "double")
    {
        if (lhs_type != "error" && rhs_type != "error") // Blocking cascade error
            error("type error in binary expression $lhs " + op + " $rhs : $lhs is \'" + lhs_type + "\' while rhs is \'" + rhs_type + "\'");
        type = "error";
        return NULL;
    }
    // If one is double, the op should also be a double op
    if (lhs_type == "double" || rhs_type == "double")
        double_flag = true;
    // If only one is double but the other is int, convert the int -> double...
    if (double_flag && lhs_type != "double")
        l = builder.CreateSIToFP(l, Type::getDoubleTy(context));
    if (double_flag && rhs_type != "double")
        r = builder.CreateSIToFP(r, Type::getDoubleTy(context));
    // Record `type`
    if (double_flag)
        type = "double";
    else
        type = "int";

    if (!double_flag)
    {
        switch (op[0])
        {
        case '+':
            return builder.CreateAdd(l, r);
        case '-':
            return builder.CreateSub(l, r);
        case '*':
            return builder.CreateMul(l, r);
        case '/':
            return builder.CreateSDiv(l, r);
        case '%':
            return builder.CreateSRem(l, r);
        case '&':
            if (op.size() == 1)
                return builder.CreateAnd(l, r);
            else
            {
                ret = builder.CreateICmpUGT(builder.CreateAnd(l, r), ConstantInt::get(Type::getInt32Ty(context), 0));
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
        case '|':
            if (op.size() == 1)
                return builder.CreateOr(l, r);
            else
            {
                ret = builder.CreateICmpUGT(builder.CreateOr(l, r), ConstantInt::get(Type::getInt32Ty(context), 0));
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
        case '^':
            return builder.CreateXor(l, r);
        case '<':
            if (op.size() == 1)
            {
                ret = builder.CreateICmpSLT(l, r);
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '=')
            {
                ret = builder.CreateICmpSLE(l, r);
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '<')
            {
                return builder.CreateShl(l, r);
            }
        case '>':
            if (op.size() == 1)
            {
                ret = builder.CreateICmpSGT(l, r);
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '=')
            {
                ret = builder.CreateICmpSGE(l, r);
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '>')
                return builder.CreateAShr(l, r);
        case '=':
        {
            ret = builder.CreateICmpEQ(l, r);
            ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
            // ret = builder.CreateUIToFP(ret, Type::getDoubleTy(context));
            // ret = builder.CreateFPToSI(ret, Type::getInt32Ty(context));
            return ret;
        }
        case '!':
        {
            ret = builder.CreateICmpNE(l, r);
            ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
            return ret;
        }
        }
    }
    else
    {
        switch (op[0])
        {
        case '+':
            return builder.CreateFAdd(l, r, "add");
        case '-':
            return builder.CreateFSub(l, r, "sub");
        case '*':
            return builder.CreateFMul(l, r, "mult");
        case '/':
            return builder.CreateFDiv(l, r, "div");
        case '>':
            if (op.size() == 1)
            {
                ret = builder.CreateFCmpUGT(l, r, "");
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '=')
            {
                ret = builder.CreateFCmpUGE(l, r, "");
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else
                error("shift operator \'>>\' not applicable to type \'double\'!\n");
        case '<':
            if (op.size() == 1)
            {
                ret = builder.CreateFCmpULT(l, r, "cmp");
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else if (op[1] == '=')
            {
                ret = builder.CreateFCmpULE(l, r, "cmp");
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
            else error("shift operator \'<<\' not applicable to type \'double\'!\n");
        default:
            return NULL;
        }
    }
    return ret;
}
llvm::Value *Nconstant::codeGen()
{
    if (type == "char")
        return builder.getInt8(value.char_value);
    else if (type == "int")
    {
        return builder.getInt32(value.int_value);
    }
    else if (type == "double")
        return ConstantFP::get(context, APFloat(value.double_value));

    else if (type == "string_literal")
    {
        string op(value.string_literal_value);
        auto str = ConstantDataArray::getString(context, op);
        auto addr = builder.CreateAlloca(str->getType(), ConstantExpr::getSizeOf(str->getType()), "str_addr");
#ifdef MACOS
        addr->setAlignment(Align(1));
#else
        addr->setAlignment(1);
#endif
        llvm::Value *p = builder.CreateGEP(addr, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
#ifdef MACOS
        builder.CreateStore(str, p)->setAlignment(Align(1));
#else
        builder.CreateStore(str, p)->setAlignment(1);
#endif
        return p;
    }
    return NULL;
}
Value *Nidentifier::codeGen()
{
    return NULL;
    // should all be over at postfix
}
Value *Ninitializer::codeGen()
{
    return assign_expr->codeGen();
}
Value *NtypeSpecifier::codeGen()
{
    return NULL;
}
Value *NinitDeclarator::codeGen()
{
    return NULL;
}
Value *NdirectDeclarator::codeGen()
{
    return NULL;
}
// Value *Ndeclarator::codeGen()
// {
//     return NULL;
// }
Value *Ndeclaration::codeGen()
{
    void *ret;
    auto type = type_specifier->type;
    for (auto iterator : init_declarator_list)
    {
        NdirectDeclarator *it = dynamic_cast<NdirectDeclarator *>(iterator);
        AllocaInst *allocation;
        auto op = it->identifier->name;
        if (it->op == "")
        {
            if (type == "double")
            {
                allocation = builder.CreateAlloca(Type::getDoubleTy(context), NULL, op);
                if (it->initializer)
                    builder.CreateStore(it->initializer->codeGen(), allocation);
            }
            else if (type == "int")
            {
                allocation = builder.CreateAlloca(Type::getInt32Ty(context), NULL, op);
                builder.CreateStore(it->initializer ? it->initializer->codeGen() : builder.getInt32(0), allocation);
            }
            else if (type == "char")
            {
                allocation = builder.CreateAlloca(Type::getInt8Ty(context), NULL, op);
                builder.CreateStore(it->initializer ? it->initializer->codeGen() : builder.getInt8(0), allocation);
            }
            ret = allocation;
            // builder.CreateStore(builder.getInt64(0), allocation);
        }
        else if (it->op[0] == '[')
        {
            Value *size = ConstantInt::get(Type::getInt32Ty(context), 1);
            int i = 0;
            for (auto constant : it->dimensions)
            {
                if (constant)
                {
                    size = builder.CreateMul(size, constant->codeGen());
                    // FIXME: negative size
                }
                i++;
            }
            Value *p;
            if (type == "double")
            {
                allocation = builder.CreateAlloca(Type::getDoubleTy(context), size, op);
                p = builder.CreateGEP(allocation, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
            }
            else if (type == "int")
            {
                allocation = builder.CreateAlloca(Type::getInt32Ty(context), size, op);
                p = builder.CreateGEP(allocation, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
            }
            else if (type == "char")
            {
                allocation = builder.CreateAlloca(Type::getInt8Ty(context), size, op);
                p = builder.CreateGEP(allocation, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
            }
            ret = p;
            dimensionBindings.insert(make_pair(op,&it->dimensions));
        }
        else if (it->op[0] == '(')
        {
            error("function definition in functions is not allowed");
            return NULL;
        }
        bindings[op]=allocation;
    }
    return (Value *)ret; // some arbitary pointer other than NULL
}
Value *NcompoundStatement::codeGen()
{
    Value *tmp;
    if (declaration_list.size())
        for (auto it = declaration_list.begin(); it != declaration_list.end(); it++)
            tmp = (*it)->codeGen();
    if (statement_list.size())
        for (auto it = statement_list.begin(); it != statement_list.end(); it++)
            tmp = (*it)->codeGen();
    return tmp;
}
Value *NifStatement::codeGen()
{
    Value *cond_val = cond_expr->codeGen();
    if (!cond_val)
        error("conditional expression is not valid!\n");

    // If the expr is not a double, convert it to a double
    if (!cond_val->getType()->isDoubleTy())
    {
        // std::cout << "Warning: condition expression in IF-ELSE statement is of type \'" << GET_VALUE_TYPE(cond_val) << "\'" << std::endl;
        cond_val = builder.CreateSIToFP(cond_val, Type::getDoubleTy(context));
    }

    // cond_val = builder.CreateICmpNE(cond_val, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true), "ifcond");
    cond_val = builder.CreateFCmpONE(cond_val, ConstantFP::get(context, APFloat(0.0)), "ifcond");

    Function *the_function = builder.GetInsertBlock()->getParent();

    BasicBlock *then_bb = BasicBlock::Create(context, "then", the_function);
    BasicBlock *else_bb = BasicBlock::Create(context, "else");
    BasicBlock *merge_bb = BasicBlock::Create(context, "ifcont");

    Value *ret = builder.CreateCondBr(cond_val, then_bb, else_bb);

    builder.SetInsertPoint(then_bb);           // set insert point to `then_bb`
    Value *then_val = if_statement->codeGen(); // recursively codeGen()
    if (!then_val)
        error("if statement is not valid!\n");
    builder.CreateBr(merge_bb);         // unconditional branch to the merge point
    then_bb = builder.GetInsertBlock(); // update `then_bb`

    the_function->getBasicBlockList().push_back(else_bb); // push `else_bb` into the BB list
    builder.SetInsertPoint(else_bb);                      // set insert point to `else_bb`
    Value *else_val = NULL;
    if (else_statement)
        else_val = else_statement->codeGen(); // recursively codeGen()
    builder.CreateBr(merge_bb);               // unconditional branch to the merge point
    else_bb = builder.GetInsertBlock();       // update `else_bb`

    the_function->getBasicBlockList().push_back(merge_bb); // push `merge_bb` into the BB list
    builder.SetInsertPoint(merge_bb);                      // set insert point to `merge_bb`

    return ret;
}
Value *NforStatement::codeGen()
{
    std::string identifier_name = identifier->name;

    Value* start_val = start_expr->codeGen();
    Value* end_val = end_expr->codeGen();
    if(!start_val)
        error("Invalid start expression in FOR statement!");
    if(!end_val)
        error("Invalid end expression in FOR statement!");
    if(GET_VALUE_TYPE(start_val) != "int")
        error("Start expression in FOR statement should be of type \'int\'!");
    if(GET_VALUE_TYPE(end_val) != "int")
        error("End expression in FOR statement should be of type \'int\'!");
    Function *the_function = builder.GetInsertBlock()->getParent();
    BasicBlock *preheader_bb = builder.GetInsertBlock();
    BasicBlock *loop_bb = BasicBlock::Create(context, "loop", the_function);
    
    // Unconditional branch to `loop_bb`
    builder.CreateBr(loop_bb);
    
    // Set insert point to `loop_bb`
    builder.SetInsertPoint(loop_bb);
    
    // Create a PHI node for the variable
    PHINode *variable = builder.CreatePHI(Type::getInt32Ty(context), 2, identifier_name);
    variable->addIncoming(start_val, preheader_bb);

    // Fetch the variable's allocation
    llvm::AllocaInst *variable_allocation = NULL, *old_variable_allocation = NULL;
    if(bindings[identifier_name] != NULL) // If there is already a binding, save the old allocation
    {
        std::cout << "Variable \'"+identifier_name+"\' already exists, of type " << GET_TYPE(identifier_name) << std::endl;
        old_variable_allocation = bindings[identifier_name];
    }
    // Allocate a new space for the loop variable
    variable_allocation = builder.CreateAlloca(Type::getInt32Ty(context), NULL, identifier_name);
    // Add to binding map
    bindings[identifier_name] = variable_allocation;
    // Store initial value
    builder.CreateStore(variable, variable_allocation);
    // Set the identifier's `type` (IMPORTANT but ugly)
    identifier->type = "int";

    // Body statement `codeGen()`
    if(!statement->codeGen())
    {
        std::cout << "Warning: Empty body statement in FOR statement!" << std::endl;
        // return NULL;
    }

    // Step is 1
    Value *step_val = builder.getInt32(1);

    // Next Variable
    Value *next_variable = inc?builder.CreateAdd(variable, step_val, "next_variable"):\
                               builder.CreateSub(variable, step_val, "next_variable");

    // Convert condition to a bool by comparing non-equal to 0
    Value* end_cond;
    end_cond = inc?builder.CreateICmpSGT(next_variable, end_val):\
                   builder.CreateICmpSLT(next_variable, end_val);
    end_cond = builder.CreateICmpEQ(end_cond, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true), "loopcond");
    
    // Create 2 new blocks
    BasicBlock *loopend_bb = builder.GetInsertBlock();
    BasicBlock *after_bb = BasicBlock::Create(context, "afterloop", the_function);

    // Insert the conditional branch into the end of `loopend_bb`
    Value* ret = builder.CreateCondBr(end_cond, loop_bb, after_bb);

    // Any new code will be inserted in `after_bb`
    builder.SetInsertPoint(after_bb);
    // Add the other incoming
    variable->addIncoming(next_variable, loopend_bb);

    if (old_variable_allocation) // Restore the unshadowed variable ('s allocation)
    {
        std::cout << "(debug) Restoring variable \'" << identifier_name << "\'" << std::endl;
        bindings[identifier_name] = old_variable_allocation;
    }
    else // No need to restore, just erase the loop variable
    {
        std::cout << "(debug) No need to restore variable \'" << identifier_name << "\'" << std::endl;
        bindings.erase(identifier_name);
    }
    return ret;
}
Value *NfunctionDefinition::codeGen()
{
    string op = direct_declarator->identifier->name;
    auto type = type_specifier->type;
    Node *body = compound_statement;

    Function *func = topModule->getFunction(op);
    vector<Type *> args;
    vector<string> argNames;
    if (!func)
    {
        int i = 0;
        for (auto it : direct_declarator->parameter_list)
        {
            args.push_back(TRANSLATE_STRING2LLVMTYPE(it->type_specifier->type));
            argNames.push_back(it->direct_declarator->identifier->name);
        }
        FunctionType *ft = FunctionType::get(TRANSLATE_STRING2LLVMTYPE(type), args, false);
        func = Function::Create(ft, Function::ExternalLinkage, op, topModule);
        // funcStack.push_back(func);
    }

    if (body)
    {
        BasicBlock *bb = BasicBlock::Create(context, "entry@" + op, func);
        builder.SetInsertPoint(bb);
        int i = 0;
        for (auto it = func->arg_begin(); it != func->arg_end(); it++)
        {
            it->setName(argNames[i]);
            auto allocation = builder.CreateAlloca(it->getType(), NULL, argNames[i]);
            builder.CreateStore(it, allocation);
            bindings[argNames[i++]] = allocation;
        }
        if (auto ret = body->codeGen())
        {
            builder.CreateRet(ret);
            verifyFunction(*func);

            return ret;
        }
        // if (func)
        //     func->eraseFromParent();
    }
    // funcStack.pop_back();
    return NULL;
}
Value *Nprogram::codeGen()
{
    for (auto it : external_declaration_list)
    {
        it->codeGen();
    }
    return NULL;
}
Value *NexprStatement::codeGen()
{
    return expr->codeGen();
}
Value *Nexpr::codeGen()
{
    Value *tmp;
    for (auto it : expr_list)
    {
        tmp = it->codeGen();
    }
    return tmp;
}
inline Value *createOpNode(Value *l, Value *r, char op)
{
    switch (op)
    {
    case '+':
        return builder.CreateFAdd(l, r, "add");
    case '-':
        return builder.CreateFSub(l, r, "sub");
    case '*':
        return builder.CreateFMul(l, r, "mult");
    case '/':
        return builder.CreateFDiv(l, r, "div");
    case '|':
        return builder.CreateOr(l, r, "or");
    case '&':
        return builder.CreateAnd(l, r, "and");
    case '%':
        return builder.CreateFRem(l, r, "");
    default:
        return NULL;
    }
}
Value *NpostfixExpr::codeGen()
{
    if (!name)
    {
        error("the reference in postfix expression is not defined");
        return NULL;
    }
    string &op = name->name;

    if (postfix_type == PARENTHESES)
    {
        type = GET_FUNCTION_TYPE(op); // bind type
        if (op == "printf")
            CreatePrintf();
        Function *callee = topModule->getFunction(op);
        vector<Value *> argv;
        for (auto it : argument_expr_list)
        {
            Value* arg_val = it->codeGen();
            if(arg_val == NULL)
                error("invalid argument in function \'" + op + "\'");
            argv.push_back(arg_val);
        }
        std::cout << std::endl;
        return builder.CreateCall(callee, argv, "call");
    }
    else if (postfix_type == SQUARE_BRACKETS || postfix_type == NONE)
    {
        type = GET_TYPE(op); // bind type
        Value* addr= getAccess();
        return builder.CreateLoad(string_to_Type(GET_TYPE(op)),addr);
    }
    return NULL;
}
Value *NpostfixExpr::getAccess()
{
    string &op(name->name);
    if(dimensionBindings.find(op)==dimensionBindings.end())return bindings[op];
    auto dimensions=*dimensionBindings[op];
    if(expr.size()!=dimensions.size())
        error("dereferncing failed, please check on the dimensions. \n"
        "Remember pointers are not supported in this version");
    if (expr.size())
    {
        Value *index=builder.getInt32(0),*stride=builder.getInt32(1);
        for(int i=expr.size()-1;i>=0;i--){
            index=builder.CreateAdd(index,builder.CreateMul(stride,expr[i]->codeGen()));
            stride=builder.CreateMul(stride,dimensions[i]->codeGen());
        }
        ConstantFolder tmp;
        return tmp.CreateGetElementPtr(bindings[op]->getType(), (Constant *)bindings[op], index);
    }
}
Value *NassignExpr::codeGen()
{
    Value *r = assign_expr->codeGen(), *l = lhs->codeGen(), *result = NULL;
    // FIXME: possible error here
    if (assign_op[0] != '=') // TODO: extension
    {
        result = createOpNode(l, r, assign_op[0]);
        return result;
    }
    if (assign_op[0] == '=')
    {
        // auto storeAddr = bindings[lhs->identifier->name];
        auto storeAddr = lhs->getAccess();
        // We should NOT directly search in the bindings
        // We shall use some virtual method, which would automatically fetch the `storeAddr` and related info (like the size of an array)
        // Or just give up assigning to an array... :)
        result = r;
        std::string lhs_type = lhs->type;
        std::string rhs_type = assign_expr->type;
        if (lhs_type == "double" && rhs_type == "int") // automatic conversion from when doing 'double' = 'int' assignment
        {
            // std::cout << "Warning(debug): Automatic conversion when assigning \'int\' to \'double\'..." << std::endl;
            result = builder.CreateSIToFP(result, Type::getDoubleTy(context));
            type = "double";
        }
        else if (lhs_type != rhs_type) // check for type error
        {
            if (lhs_type != "error" && rhs_type != "error") // Blocking cascade error
                error("type error in assignment expression $lhs = $rhs: $lhs is \'" + lhs_type + "\' while $rhs is \'" + rhs_type + "\'");
            type = "error";
            return NULL;
        }
        else
            type = lhs_type;
        builder.CreateStore(result, storeAddr);
        return result;
    }
    return NULL;
}
Value *NcondExpr::codeGen()
{
    return NULL;
}
Value *NcastExpr::codeGen()
{
    return NULL;
}
Value *NunaryExpr::codeGen()
{
    return NULL;
}

Value *NparameterDeclaration::codeGen()
{
    return NULL;
}
llvm::Function *CreatePrintf()
{
    std::vector<llvm::Type *> arg_types;
    arg_types.push_back(builder.getInt8PtrTy());
    auto printf_type = llvm::FunctionType::get(builder.getInt32Ty(), llvm::makeArrayRef(arg_types), true);
    auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), topModule);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}
llvm::Function *CreateScanf()
{
    auto scanf_type = llvm::FunctionType::get(builder.getInt32Ty(), true);
    auto func = llvm::Function::Create(scanf_type, llvm::Function::ExternalLinkage, llvm::Twine("scanf"), topModule);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}
llvm::Value *NderivedType::codeGen()
{
    return NULL;
}
llvm::Value *NparameterList::codeGen()
{
    return NULL;
}
llvm::Value *Nstruct::codeGen()
{
    return NULL;
}
llvm::Value *NreturnStatement::codeGen()
{
    if (expr)
    {
        return builder.CreateRet(expr->codeGen());
    }
    else
        return builder.CreateRet(NULL);
}
