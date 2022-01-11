//
// Created by saheed on 1/3/22.
//

#include "CodeGen.hpp"

using namespace llvm;
using namespace kal;
using namespace std;

std::unique_ptr<llvm::LLVMContext> CodeGen::TheContext;
std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> CodeGen::Builder;
std::unique_ptr<llvm::Module> CodeGen::TheModule;
std::map<std::string, llvm::Value *> CodeGen::NamedValues;

void CodeGen::On(std::shared_ptr<IntegerExprAST> intExprAST) {
    valueResult = llvm::ConstantInt::get(Type::getInt32Ty(*TheContext), intExprAST->GetVal(), true);

//    if(!valueResult) return;
//    // print it out
//    valueResult->print(errs());
//    fprintf(stderr, "\n");
}

void CodeGen::On(std::shared_ptr<FloatExprAST> floatExprAST) {
    valueResult = ConstantFP::get(*TheContext, APFloat(floatExprAST->GetVal()));

//    if(!valueResult) return;
//    // print it out
//    valueResult->print(errs());
//    fprintf(stderr, "\n");
}

void CodeGen::On(std::shared_ptr<UnaryExprAST> uniExprAST) {
    valueResult = nullptr;
}

Value * CreateOp(char Op, Value *Left, Value *Right) {
    if (Left->getType()->isIntegerTy()) {
//        TODO: remove this when static types are supported
        // convert to double
        Left = CodeGen::Builder->
                CreateUIToFP(Left, Type::getDoubleTy(*CodeGen::TheContext), "uitofp");

/*
        if (Right->getType() != Left->getType()) {
//            Right->mutateType(Left->getType());
//            Right = CastInst::Create(llvm::Instruction::FPToUI, Right, Left->getType(), "fptoui");
            Right = CodeGen::Builder->CreateFPToUI(Right, Left->getType(), "fptoui");
        }

        switch (Op) {
            case '+':
                return CodeGen::Builder->CreateAdd(Left, Right, "iaddtmp");
            case '-':
                return CodeGen::Builder->CreateSub(Left, Right, "isubtmp");
            case '*':
                return CodeGen::Builder->CreateMul(Left, Right, "imultmp");
            case '/':
                return CodeGen::Builder->CreateUDiv(Left, Right, "idivtmp");
            case '<':
                return CodeGen::Builder->CreateFCmpULT(Left, Right, "icmplttmp");
            case '>':
                return CodeGen::Builder->CreateICmpUGT(Left, Right, "icmpgttmp");
            default:
                ErrorLogger::LogError("invalid binary operator");
                return nullptr;
        }
        */
    }

//    if (Left->getType()->isDoubleTy() || Left->getType()->isFloatTy()) {
        if (Right->getType() != Left->getType())
            Right = CodeGen::Builder->CreateUIToFP(Right, Left->getType(), "uitofp");

        switch (Op) {
            case '+':
                return CodeGen::Builder->CreateFAdd(Left, Right, "faddtmp");
            case '-':
                return CodeGen::Builder->CreateFSub(Left, Right, "fsubtmp");
            case '*':
                return CodeGen::Builder->CreateFMul(Left, Right, "fmultmp");
            case '/':
                return CodeGen::Builder->CreateFDiv(Left, Right, "fdivtmp");
            case '<':
                return CodeGen::Builder->CreateFCmpULT(Left, Right, "fcmpgttmp");
            case '>':
                // TODO: Convert bool 0/1 to double 0.0 or 1.0
                Left = CodeGen::Builder->CreateFCmpULT(Left, Right, "fcmplttmp");
                // Convert bool 0/1 to double 0.0 or 1.0
                return CodeGen::Builder->CreateUIToFP(Left, Type::getDoubleTy(*CodeGen::TheContext), "booltmp");
            default:
                ErrorLogger::LogError("invalid binary operator");
                return nullptr;
        }
//    }

//    Left->getType()->print(errs());
//    ErrorLogger::LogError("invalid type detected!");
//    return nullptr;
}

void CodeGen::On(std::shared_ptr<BinaryExprAST> binExprAST) {
    CodeGen codegen{};
//    Value *Left = binExprAST->getLHS()->Perform(codegen);
//    Value *Right = binExprAST->getRHS()->Perform(codegen);

    binExprAST->getLHS()->Perform(codegen);
    auto Left = codegen.TakeValueResult();
    binExprAST->getRHS()->Perform(codegen);
    auto Right = codegen.TakeValueResult();

    if (!Left || !Right)
        return;

    // the type of LHS prevails
    valueResult = CreateOp(binExprAST->getOp(), Left, Right);

////    if (auto *FnIR = exprAST->codegen()) {
////      if (auto *FnIR = exprAST->Perform(codegen)) {
//    if (FnIR) {
//        fprintf(stderr, "Read top-level expression:");
//        FnIR->print(errs());
//        fprintf(stderr, "\n");
//
//        // Remove the anonymous expression.
//        FnIR->eraseFromParent();
//    } else ErrorLogger::LogError("No IR was generated!");
}

//// Returns current value of Codegen::valueResult
//// and reset it to nullptr
llvm::Value *CodeGen::TakeValueResult() {
    auto ret = valueResult;
    valueResult = nullptr; // reset
    return ret;
}

//// Returns current value of Codegen::functionResult
//// and reset it to nullptr
llvm::Function *CodeGen::TakeFunctionResult() {
    auto ret = functionResult;
    functionResult = nullptr; // reset
    return ret;
}

void CodeGen::On(std::shared_ptr<PrototypeAST> prototypeAST) {
    // Make the function type:  double(double,double) etc.
    std::vector<Type *> Doubles(prototypeAST->getArgs().size(), Type::getDoubleTy(*TheContext));
    FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext),
                                         Doubles, false);

    functionResult = Function::Create(FT, Function::ExternalLinkage,
                                      prototypeAST->getName(), TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg: functionResult->args())
        Arg.setName(prototypeAST->getArgs()[Idx++]);
}

void CodeGen::On(std::shared_ptr<FunctionAST> functionAST) {
    CodeGen codegen{};
    auto proto = functionAST->getPrototype();
    auto body = functionAST->getBody();

    // First, check for an existing function from a previous 'extern' declaration.
    Function *TheFunction = TheModule->getFunction(proto->getName());

    if (!TheFunction) {
        proto->Perform(codegen);
        TheFunction = codegen.TakeFunctionResult();
    }

    if (!TheFunction)
        return;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    body->Perform(codegen);
    Value *RetVal = codegen.TakeValueResult();

    // Finish off the function.
    if (RetVal) { Builder->CreateRet(RetVal); }
    else if (TheFunction->getName() == "main")
//        Builder->CreateRet(nullptr); // return void
        Builder->CreateRet(ConstantFP::getNegativeZero(TheFunction->getReturnType()));
    else {
        ErrorLogger::LogError("Error reading body.");
        // Error reading body, remove function.
        TheFunction->eraseFromParent();
        functionResult = nullptr;
        return;
    }
    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);
    functionResult = TheFunction;
//        errs() << *TheFunction;
}

void CodeGen::On(std::shared_ptr<VariableExprAST> variableExprAST) {
    // Look this variable up in the function.
    valueResult = NamedValues[variableExprAST->getName()];
    if (!valueResult)
        ErrorLogger::LogError("Unknown variable name");
}

void CodeGen::On(std::shared_ptr<CallExprAST> callExprAST) {
    // Look up the name in the global module table.
    Function *CalleeF = TheModule->getFunction(callExprAST->getCallee());
    if (!CalleeF) {
        ErrorLogger::LogError("Unknown function referenced");
        return;
    }

    // If argument mismatch error.
    if (CalleeF->arg_size() != callExprAST->getArgs().size()) {
        ErrorLogger::LogError("Incorrect # arguments passed");
        return;
    }

    std::vector<Value *> ArgsV;
    CodeGen codeGen;
    for (auto & arg : callExprAST->getArgs()) {
        arg->Perform(codeGen);
        auto value = codeGen.TakeValueResult();
        if (value && value->getType()->isIntegerTy()){
            value = CodeGen::Builder->
                    CreateUIToFP(value, Type::getDoubleTy(*CodeGen::TheContext), "uitofp");
        }
        ArgsV.push_back(value);
        if (!ArgsV.back())
            return;
    }

    Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}