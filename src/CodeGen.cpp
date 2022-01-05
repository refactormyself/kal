//
// Created by saheed on 1/3/22.
//

#include "CodeGen.hpp"

using namespace llvm;
using namespace kal;
using namespace std;

llvm::LLVMContext CodeGen::TheContext;
std::unique_ptr<llvm::IRBuilder<>> CodeGen::Builder;

void CodeGen::On(std::shared_ptr<IntegerExprAST> intExprAST) {
    Result = llvm::ConstantInt::get(Type::getInt32Ty(TheContext), intExprAST->GetVal(), true);
}

void CodeGen::On(std::shared_ptr<FloatExprAST> floatExprAST) {
    Result = ConstantFP::get(TheContext, APFloat(floatExprAST->GetVal()));
}

void CodeGen::On(std::shared_ptr<UnaryExprAST> uniExprAST) {
    Result = nullptr;
}

Value * CreateOp(char Op, Value *Left, Value *Right) {
//    std::unique_ptr<llvm::IRBuilder<>> Builder;
    Value *Result;

    if (Left->getType()->isIntegerTy()) {
        ErrorLogger::LogError("=== OP IS INTEGER ! ===");

        if (!Right->getType()->isIntegerTy()) { // TODO: use Builder->CreateFPToUI()
            Right->mutateType(Left->getType());
            ErrorLogger::LogError("=== RHS IS Mutated ! ===");
        }
        ErrorLogger::LogError("=== ABOUT TO SWITCH OP ! ===");

        switch (Op) {
            case '+':
                ErrorLogger::LogError("=== ABOUT TO + ===");
                Left->print(errs());
                Right->print(errs());
                Result = CodeGen::Builder->CreateAdd(Left, Right, "iaddtmp");
                ErrorLogger::LogError("=== DONE WITH + ===");
                return Result;
            case '-':
                Result = CodeGen::Builder->CreateSub(Left, Right, "isubtmp");
                return Result;
            case '*':
                Result = CodeGen::Builder->CreateMul(Left, Right, "imultmp");
                return Result;
            case '/':
                Result = CodeGen::Builder->CreateUDiv(Left, Right, "idivtmp");
                return Result;
            case '<':
                Result = CodeGen::Builder->CreateICmpULT(Left, Right, "icmplttmp");
                return Result;
            case '>':
                Result = CodeGen::Builder->CreateICmpUGT(Left, Right, "icmpgttmp");
                return Result;
            default:
                ErrorLogger::LogError("invalid binary operator");
                return nullptr;
        }
    }

    if (Left->getType()->isDoubleTy() || Left->getType()->isFloatTy()) {
        ErrorLogger::LogError("=== OP IS FLOAT ! ===");

        if (!(Right->getType()->isDoubleTy() || Right->getType()->isFloatTy())) // TODO: use Builder->CreateUIToFP()
            Right->mutateType(Left->getType());

        switch (Op) {
            case '+':
                Result = CodeGen::Builder->CreateFAdd(Left, Right, "faddtmp");
                return Result;
            case '-':
                Result = CodeGen::Builder->CreateFSub(Left, Right, "fsubtmp");
                return Result;
            case '*':
                Result = CodeGen::Builder->CreateFMul(Left, Right, "fmultmp");
                return Result;
            case '/':
                Result = CodeGen::Builder->CreateFDiv(Left, Right, "fdivtmp");
                return Result;
            case '<':
                Result = CodeGen::Builder->CreateFCmpULT(Left, Right, "fcmplttmp");
                // Convert bool 0/1 to double 0.0 or 1.0
                Result = CodeGen::Builder->CreateUIToFP(Result, Type::getDoubleTy(CodeGen::TheContext), "fconvtmp");
                return Result;
            case '>':
                Result = CodeGen::Builder->CreateFCmpUGT(Left, Right, "fcmpgttmp");
                // Convert bool 0/1 to double 0.0 or 1.0
                Result = CodeGen::Builder->CreateUIToFP(Result, Type::getDoubleTy(CodeGen::TheContext), "fconvtmp");
                return Result;
            default:
                ErrorLogger::LogError("invalid binary operator");
                return nullptr;
        }
    }

    Left->getType()->print(errs());
    ErrorLogger::LogError("invalid type detected!");
    return nullptr;
}

void CodeGen::On(std::shared_ptr<BinaryExprAST> binExprAST) {
    CodeGen codegen{};
//    Value *Left = binExprAST->getLHS()->Perform(codegen);
//    Value *Right = binExprAST->getRHS()->Perform(codegen);

    binExprAST->getLHS()->Perform(codegen);
    auto Left = codegen.GetResult();
    binExprAST->getRHS()->Perform(codegen);
    auto Right = codegen.GetResult();

    if (!Left || !Right)
        return;

    ErrorLogger::LogError("=== ABOUT TO CREATE OP ! ===");
    // the type of LHS prevails
    auto FnIR = CreateOp(binExprAST->getOp(), Left, Right);

    ErrorLogger::LogError("===|++++=====|");

//    if (auto *FnIR = exprAST->codegen()) {
//      if (auto *FnIR = exprAST->Perform(codegen)) {
    if (FnIR) {
        fprintf(stderr, "Read top-level expression:");
        FnIR->print(errs());
        fprintf(stderr, "\n");

        // Remove the anonymous expression.
//        FnIR->eraseFromParent();
    }

}

llvm::Value *CodeGen::GetResult() {
    return Result;
}
