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
    type = "NULL";

    if (!lhs)
    {
        ERROR("invalid $lhs in binary expression $lhs " + op + " $rhs");
        return NULL;
    }
    if (!rhs)
    {
        ERROR("invalid $rhs in binary expression $lhs " + op + " $rhs");
        return NULL;
    }

    std::string lhs_type = lhs->type;
    std::string rhs_type = rhs->type;

    // If one is double, the op should also be a double op
    if (lhs_type == "double" && rhs_type == "double")
        type = "double";
    else if(lhs_type == "int" && rhs_type == "double") // convert int -> double
    {
        type = "double";
        l = builder.CreateSIToFP(l, Type::getDoubleTy(context));
    }
    else if(lhs_type == "double" && rhs_type == "int") // convert int -> double
    {
        type = "double";
        r = builder.CreateSIToFP(r, Type::getDoubleTy(context));
    }
    else if(lhs_type == "int" && rhs_type == "int")
        type = "int";
    else if(lhs_type == "char" && rhs_type == "char")
    {
        type = "int";
        l = builder.CreateIntCast(l, Type::getInt32Ty(context), false);
        r = builder.CreateIntCast(r, Type::getInt32Ty(context), false);
    }
    else if(lhs_type == "char" && rhs_type == "int") // convert char -> int
    {
        type = "int";
        l = builder.CreateIntCast(l, Type::getInt32Ty(context), false);
    }
    else if(lhs_type == "int" && rhs_type == "char") // convert char -> int
    {
        type = "int";
        r = builder.CreateIntCast(r, Type::getInt32Ty(context), false);
    }
    else // check for type error
    {
        type = "error";
        if (lhs_type != "error" && rhs_type != "error") // Blocking cascade error
            ERROR("type error in binary expression $lhs " + op + " $rhs : $lhs is \'" + lhs_type + "\' while rhs is \'" + rhs_type + "\'");
        return NULL;
    }


    if (type == "int" || type == "char")
    {
        Value *constant_zero;
        if(type == "int" || type == "char") constant_zero = ConstantInt::get(Type::getInt32Ty(context), 0);
        // else if(type == "char") constant_zero = ConstantInt::get(Type::getInt8Ty(context), 0);

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
                ret = builder.CreateICmpUGT(builder.CreateAnd(l, r), constant_zero);
                ret = builder.CreateIntCast(ret, Type::getInt32Ty(context), false);
                return ret;
            }
        case '|':
            if (op.size() == 1)
                return builder.CreateOr(l, r);
            else
            {
                ret = builder.CreateICmpUGT(builder.CreateOr(l, r), constant_zero);
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
        default:
        {
            ERROR("invalid binary operator \'" + op + "\' for type \'int\'", 0);
            return NULL;
        }
        }
    }
    else if(type == "double")
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
            {
                ERROR("shift operator \'>>\' not applicable to type \'double\'!\n");
                return NULL;
            }
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
            else
            {
                ERROR("shift operator \'<<\' not applicable to type \'double\'!\n");
                return NULL;
            }
        default:
            ERROR("invalid binary operator \'" + op + "\' for type \'double\'", 0);
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
        Value *allocation;
        auto op = it->identifier->name;
        if (it->op == "") // A single identifier
        {
            if(is_global) // if a global variable declaration
            {
                global_variables_type[op] = type; // manually bind
                std::cout << "Bind '" << op << "' successfully to type '" << type << "'" << std::endl;
                Value* constant_zero; // get constant zero as the default initializer
                if(type == "int") constant_zero = ConstantInt::get(Type::getInt32Ty(context), 0);
                else if(type == "char") constant_zero = ConstantInt::get(Type::getInt8Ty(context), 0);
                else if(type == "double") constant_zero = ConstantFP::get(context, APFloat(0.0));
                
                if(it->initializer) // if an initializer already exists
                {
                    if(!it->initializer->assign_expr->is_constant)
                    {
                        ERROR("only constants are allowed to initialize a global variable");
                        return NULL;
                    }
                    if(it->initializer->assign_expr->type != type)
                    {
                        if(it->initializer->assign_expr->type == "int" && type == "double")
                        {
                            allocation = new llvm::GlobalVariable(*topModule, string_to_Type(type), false, llvm::GlobalValue::ExternalLinkage,\
                                                          (Constant*)(builder.CreateSIToFP(it->initializer->codeGen(), Type::getDoubleTy(context))), op);
                            continue;
                        }
                        if(it->initializer->assign_expr->type == "int" && type == "char")
                        {
                            allocation = new llvm::GlobalVariable(*topModule, string_to_Type(type), false, llvm::GlobalValue::ExternalLinkage,\
                                                                  (Constant*)(builder.CreateIntCast(it->initializer->codeGen(), Type::getInt8Ty(context), false)), op);
                            continue;
                        }
                        ERROR("must initialize \'" + type + " " + op + " with a legal type");
                        return NULL;
                    }
                    allocation = new llvm::GlobalVariable(*topModule, string_to_Type(type), false, llvm::GlobalValue::ExternalLinkage, (Constant*)(it->initializer->codeGen()), op);
                }
                else allocation = new llvm::GlobalVariable(*topModule, string_to_Type(type), false, llvm::GlobalValue::ExternalLinkage, (Constant*)constant_zero, op);
                Value* tmp = topModule->getNamedGlobal(op);
                std::cout << "Global type: " << GET_VALUE_TYPE(tmp) << std::endl;
                continue;
            }
            // Otherwise a local variable
            if (type == "double")
            {   
                allocation = builder.CreateAlloca(Type::getDoubleTy(context), NULL, op);
                if(!allocation)
                {
                    ERROR("unable to allocate for variable \'" + op + "\' of type \'" + type + "\'");
                    return NULL;
                }
                if (it->initializer)
                    builder.CreateStore(it->initializer->codeGen(), allocation);
            }
            else if (type == "int")
            {
                allocation = builder.CreateAlloca(Type::getInt32Ty(context), NULL, op);
                if(!allocation)
                {
                    ERROR("unable to allocate for variable \'" + op + "\' of type \'" + type + "\'");
                    return NULL;
                }
                builder.CreateStore(it->initializer ? it->initializer->codeGen() : builder.getInt32(0), allocation);
            }
            else if (type == "char")
            {
                allocation = builder.CreateAlloca(Type::getInt8Ty(context), NULL, op);
                if(!allocation)
                {
                    ERROR("unable to allocate for variable \'" + op + "\' of type \'" + type + "\'");
                    return NULL;
                }
                builder.CreateStore(it->initializer ? it->initializer->codeGen() : builder.getInt8(0), allocation);
            }
            ret = allocation;
            // builder.CreateStore(builder.getInt64(0), allocation);
        }
        else if (it->op[0] == '[') // An array
        {
            int num = 1;
            Value *size = ConstantInt::get(Type::getInt32Ty(context), 1);
            int i = 0;
            for (auto constant : it->dimensions)
            {
                if (constant)
                {
                    num *= constant->value.int_value;
                    size = builder.CreateMul(size, constant->codeGen());
                    // FIXME: negative size
                }
                i++;
            }
            ArrayType* array_type = ArrayType::get(string_to_Type(type), num);
            Value *p;

            if(is_global) // if a global variable declaration
            {
                global_variables_type[op] = type; // manually bind
                Value *constant_zero;
                if(type == "int") constant_zero = ConstantInt::get(Type::getInt32Ty(context), 0);
                else if(type == "char") constant_zero = ConstantInt::get(Type::getInt8Ty(context), 0);
                allocation = new llvm::GlobalVariable(*topModule, array_type, false, llvm::GlobalValue::CommonLinkage, (Constant*)constant_zero, op);
                // ConstantAggregateZero* const_array_2 = ConstantAggregateZero::get(ArrayTy_0);
                // allocation->setInitializer(const_array_2);
                p = allocation; 
            }
            else // otherwise a local array declaration
            {
                allocation = builder.CreateAlloca(array_type, NULL, op);
                p = builder.CreateGEP(allocation, ConstantInt::get(Type::getInt32Ty(context), 0), "tmp");
            }
            
            ret = p;
            dimensionBindings.insert(make_pair(op, &it->dimensions));
        }
        else if (it->op[0] == '(') // A function definition
        {
            Function *func = topModule->getFunction(op);
            vector<Type *> args;
            vector<string> argNames;
            if (!func)
            {
                int i = 0;
                for (auto j : it->parameter_list)
                {
                    args.push_back(string_to_Type(j->type_specifier->type));
                    argNames.push_back(j->direct_declarator->identifier->name);
                }
                FunctionType *ft = FunctionType::get(string_to_Type(type), args, false);
                func = Function::Create(ft, Function::ExternalLinkage, op, topModule);
                // funcStack.push_back(func);
            }
            // ERROR("function definition in functions is not allowed");
        }
        bindings[op] = allocation;
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
    {
        ERROR("conditional expression is not valid!\n");
        return NULL;
    }

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
    {
        ERROR("if statement is not valid!\n");
        return NULL;
    }
    builder.CreateBr(merge_bb);         // unconditional branch to the merge point
    then_bb = builder.GetInsertBlock(); // update `then_bb`

    the_function->getBasicBlockList().push_back(else_bb); // push `else_bb` into the BB list
    builder.SetInsertPoint(else_bb);                      // set insert point to `else_bb`
    Value *else_val = NULL;
    if (else_statement)
    {
        else_val = else_statement->codeGen(); // recursively codeGen()
        if (!else_val)
        {
            ERROR("2nd body statement of \'if\' statement is not valid");
            return NULL;
        }
    }
    builder.CreateBr(merge_bb);         // unconditional branch to the merge point
    else_bb = builder.GetInsertBlock(); // update `else_bb`

    the_function->getBasicBlockList().push_back(merge_bb); // push `merge_bb` into the BB list
    builder.SetInsertPoint(merge_bb);                      // set insert point to `merge_bb`

    return ret;
}
Value *NforStatement::codeGen()
{
    std::string identifier_name = identifier->name;

    Value *start_val = start_expr->codeGen();
    Value *end_val = end_expr->codeGen();
    if (!start_val)
    {
        ERROR("Invalid start expression in FOR statement!");
        return NULL;
    }
    if (!end_val)
    {
        ERROR("Invalid end expression in FOR statement!");
        return NULL;
    }
    if (GET_VALUE_TYPE(start_val) != "int")
    {
        ERROR("Start expression in FOR statement should be of type \'int\'!");
        return NULL;
    }
    if (GET_VALUE_TYPE(end_val) != "int")
    {
        ERROR("End expression in FOR statement should be of type \'int\'!");
        return NULL;
    }
    Function *the_function = builder.GetInsertBlock()->getParent();
    BasicBlock *preheader_bb = builder.GetInsertBlock();
    BasicBlock *loop_bb = BasicBlock::Create(context, "loop", the_function);
    BasicBlock *after_bb = BasicBlock::Create(context, "afterloop", the_function);

    // Check if the condition is met initially; otherwise don't even need to enter the loop
    Value *enterloop_cond;
    enterloop_cond = inc ? builder.CreateICmpSGT(start_val, end_val) : builder.CreateICmpSLT(start_val, end_val);
    enterloop_cond = builder.CreateICmpEQ(enterloop_cond, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true), "enterloop_cond");
    // Conditional branch to `loop_bb`
    builder.CreateCondBr(enterloop_cond, loop_bb, after_bb);

    // Set insert point to `loop_bb`
    builder.SetInsertPoint(loop_bb);

    // Create a PHI node for the variable
    PHINode *variable = builder.CreatePHI(Type::getInt32Ty(context), 2, identifier_name);
    variable->addIncoming(start_val, preheader_bb);

    // Fetch the variable's allocation
    llvm::AllocaInst *variable_allocation = NULL, *old_variable_allocation = NULL;
    if (bindings[identifier_name] != NULL) // If there is already a binding, save the old allocation
    {
        std::cout << "Variable \'" + identifier_name + "\' already exists, of type " << GET_TYPE(identifier_name) << std::endl;
        old_variable_allocation = (llvm::AllocaInst*)bindings[identifier_name];
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
    if (!statement->codeGen())
    {
        std::cout << "Warning: empty body statement in \'for\' statement!" << std::endl;
        // return NULL;
    }

    // Get next variable
    // Step is 1
    Value *step_val = builder.getInt32(1);
    // Next variable
    Value* cur_variable = builder.CreateLoad((AllocaInst*)variable_allocation);
    Value *next_variable = inc ? builder.CreateAdd(cur_variable, step_val, "next_variable") : builder.CreateSub(variable, step_val, "next_variable");

    // Convert condition to a bool by comparing non-equal to 0
    Value *continue_cond;
    continue_cond = inc ? builder.CreateICmpSGT(next_variable, end_val) : builder.CreateICmpSLT(next_variable, end_val);
    continue_cond = builder.CreateICmpEQ(continue_cond, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true), "continue_cond");

    // get the block where the loop ends
    BasicBlock *loopend_bb = builder.GetInsertBlock();

    // Insert the conditional branch into the end of `loopend_bb`
    Value *ret = builder.CreateCondBr(continue_cond, loop_bb, after_bb);

    // Any new code will be inserted in `after_bb`
    builder.SetInsertPoint(after_bb);

    // Add the other incoming
    variable->addIncoming(next_variable, loopend_bb);

    if (old_variable_allocation) // Restore the old variable ('s allocation)
    {
        std::cout << "(debug) Restoring variable \'" << identifier_name << "\'" << std::endl;
        bindings[identifier_name] = old_variable_allocation;
    }
    else // No need to restore, just erase the loop variable
    {
        std::cout << "(debug) No need to restore variable \'" << identifier_name << "\'" << std::endl;
        // bindings.erase(identifier_name);
    }
    return ret;
}
Value *NwhileStatement::codeGen()
{
    Function *the_function = builder.GetInsertBlock()->getParent();
    BasicBlock *cond_bb = BasicBlock::Create(context, "cond", the_function);

    // While condition block
    builder.CreateBr(cond_bb);
    builder.SetInsertPoint(cond_bb);
    Value *while_cond = cond_expr->codeGen();
    if (GET_VALUE_TYPE(while_cond) == "char")
        while_cond = builder.CreateICmpEQ(while_cond, llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), 1, true), "whilecond");
    else if (GET_VALUE_TYPE(while_cond) == "int")
        while_cond = builder.CreateICmpEQ(while_cond, llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1, true), "whilecond");
    else if (GET_VALUE_TYPE(while_cond) == "double")
        while_cond = builder.CreateFCmpONE(while_cond, ConstantFP::get(context, APFloat(1.0)), "whilecond");
    else
    {
        ERROR("invalid condition expression in \'while\' statement");
        return NULL;
    }

    BasicBlock *loop_bb = BasicBlock::Create(context, "loop", the_function);
    BasicBlock *after_bb = BasicBlock::Create(context, "afterloop", the_function);
    Value *ret = builder.CreateCondBr(while_cond, loop_bb, after_bb);

    // Loop block
    builder.SetInsertPoint(loop_bb);
    if (!statement->codeGen())
    {
        std::cout << "Warning: empty body statement in \'while\' statement!" << std::endl;
    }
    builder.CreateBr(cond_bb); // goto cond_bb after each loop

    // After loop block
    builder.SetInsertPoint(after_bb);

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
            args.push_back(string_to_Type(it->type_specifier->type));
            // argNames.push_back(it->direct_declarator->identifier->name);
        }
        FunctionType *ft = FunctionType::get(string_to_Type(type), args, false);
        func = Function::Create(ft, Function::ExternalLinkage, op, topModule);
        // funcStack.push_back(func);
    }

    if (body)
    {
        BasicBlock *bb = BasicBlock::Create(context, "entry@" + op, func);
        builder.SetInsertPoint(bb);
        for (auto it : direct_declarator->parameter_list)
        {
            // args.push_back(string_to_Type(it->type_specifier->type));
            argNames.push_back(it->direct_declarator->identifier->name);
        }
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
    CreateScanf();
    CreatePrintf();
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
        ERROR("reference in postfix expression is not defined");
        return NULL;
    }
    string &op = name->name;

    if (postfix_type == PARENTHESES)
    {
        type = GET_FUNCTION_TYPE(op); // bind type
        Function *callee = topModule->getFunction(op);
        vector<Value *> argv;
        int init = 1;
        for (auto it : argument_expr_list)
        {
            Value *arg_val = it->codeGen();
            if (arg_val == NULL)
            {
                ERROR("invalid argument in function \'" + op + "\'");
                return NULL;
            }
            if (op != "scanf" || init)
            {
                init = 0;
                argv.push_back(arg_val);
            }
            else
            {
                auto target = dynamic_cast<NpostfixExpr *>(it);
                if (!target)
                {
                    ERROR("require a postfix expression in the parameter list of `scanf()`");
                    return NULL;
                }
                argv.push_back(builder.CreateGEP(target->getAccess(), ConstantInt::get(Type::getInt32Ty(context), 0), "tmp"));
            }
        }
        std::cout << std::endl;
        return builder.CreateCall(callee, argv, "call");
    }
    else if (postfix_type == SQUARE_BRACKETS || postfix_type == NONE)
    {
         // bind type
        if(topModule->getNamedGlobal(op) != NULL) type = global_variables_type[op];
        else type = GET_TYPE(op);
        // get access
        Value *addr = getAccess();
        if(addr == NULL)
        {
            ERROR("(internal) invalid address while fetching declarator \'" + op + "\'");
            return NULL;
        }
        // load variable
        Value* ret = builder.CreateLoad(string_to_Type(type), addr);
        return ret;
    }
    return NULL;
}
Value *NpostfixExpr::getAccess()
{
    string &op(name->name);
    /* Global variables have higher priviledge in RCC!!! */
    if(topModule->getNamedGlobal(op) != NULL)
    {
        /* A single variable, not an array */
        if(dimensionBindings.find(op) == dimensionBindings.end())
            return topModule->getNamedGlobal(op);
        
        /* Else a global array */
        auto dimensions = *dimensionBindings[op];
        if (expr.size() != dimensions.size())
        {
            ERROR("dereferencing failed, please check on the dimensions. \n"
                "Remember pointers are not supported in this version");
            return NULL;
        }
        if (expr.size())
        {
            // Deprecated index method with brute force below..
            Value *index = builder.getInt32(0), *stride = builder.getInt32(1);
            for (int i = expr.size() - 1; i >= 0; i--)
            {
                index = builder.CreateAdd(index, builder.CreateMul(stride, expr[i]->codeGen()));
                stride = builder.CreateMul(stride, dimensions[i]->codeGen());
            }
            // return builder.CreateGEP((Value*)(bindings[op]), index);
            ConstantFolder tmp;
            return tmp.CreateGetElementPtr(string_to_Type(global_variables_type[op]), topModule->getNamedGlobal(op), index);

            // std::vector<Value*> indices;
            // for (int i = 0; i < expr.size(); i++)
            //     indices.push_back(expr[i]->codeGen());
            // return builder.CreateGEP(topModule->getNamedGlobal(op), indices);
        }
    }
    /* Otherwise local variables */
    if (dimensionBindings.find(op) == dimensionBindings.end())
        return (llvm::AllocaInst *)bindings[op];
    /* Otherwise local arrays */
    auto dimensions = *dimensionBindings[op];
    if (expr.size() != dimensions.size())
    {
        ERROR("dereferencing failed, please check on the dimensions. \n"
              "Remember pointers are not supported in this version");
        return NULL;
    }
    if (expr.size())
    {
        // Deprecated index method with brute force below..
        Value *index = builder.getInt32(0), *stride = builder.getInt32(1);
        for (int i = expr.size() - 1; i >= 0; i--)
        {
            index = builder.CreateAdd(index, builder.CreateMul(stride, expr[i]->codeGen()));
            stride = builder.CreateMul(stride, dimensions[i]->codeGen());
        }
        // return builder.CreateGEP((Value*)(bindings[op]), index);
        ConstantFolder tmp;
        return tmp.CreateGetElementPtr(((llvm::AllocaInst *)(bindings[op]))->getType(), (Constant *)bindings[op], index);
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
        else if (lhs_type == "int" && rhs_type == "char") // automatic conversion from when doing 'int' = 'char' assignment
        {
            result = builder.CreateIntCast(result, Type::getInt32Ty(context), false);
            type = "int";
        }
        else if (lhs_type == "char" && rhs_type == "int") // automatic conversion from when doing 'int' = 'char' assignment
        {
            result = builder.CreateIntCast(result, Type::getInt8Ty(context), false);
            type = "char";
        }
        else if (lhs_type != rhs_type) // check for type error
        {
            if (lhs_type != "error" && rhs_type != "error") // Blocking cascade error
                ERROR("type error in assignment expression $lhs = $rhs: $lhs is \'" + lhs_type + "\' while $rhs is \'" + rhs_type + "\'");
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
