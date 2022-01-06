//
// Created by saheed on 1/3/22.
//

#include "CodeGen.hpp"

using namespace llvm;
using namespace kal;
using namespace std;

std::unique_ptr<llvm::LLVMContext> CodeGen::TheContext;
std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> CodeGen::Builder;

void CodeGen::On(std::shared_ptr<IntegerExprAST> intExprAST) {
    Result = llvm::ConstantInt::get(Type::getInt32Ty(*TheContext), intExprAST->GetVal(), true);

    if(!Result) return;
    // print it out
    Result->print(errs());
    fprintf(stderr, "\n");
}

void CodeGen::On(std::shared_ptr<FloatExprAST> floatExprAST) {
    Result = ConstantFP::get(*TheContext, APFloat(floatExprAST->GetVal()));

    if(!Result) return;
    // print it out
    Result->print(errs());
    fprintf(stderr, "\n");
}

void CodeGen::On(std::shared_ptr<UnaryExprAST> uniExprAST) {
    Result = nullptr;
}

Value * CreateOp(char Op, Value *Left, Value *Right) {
    Value *Result;

    if (Left->getType()->isIntegerTy()) {
        if (Right->getType() != Left->getType()) {
//            Right->mutateType(Left->getType());
//            Right = CastInst::Create(llvm::Instruction::FPToUI, Right, Left->getType(), "fptoui");
            Right = CodeGen::Builder->CreateFPToUI(Right, Left->getType(), "fptoui");
        }

        switch (Op) {
            case '+':
                return BinaryOperator::CreateAdd(Left, Right, "iaddtmp");
            case '-':
                return BinaryOperator::CreateSub(Left, Right, "isubtmp");
            case '*':
                return BinaryOperator::CreateMul(Left, Right, "imultmp");
            case '/':
                return BinaryOperator::CreateUDiv(Left, Right, "idivtmp");
            case '<':
                return new ICmpInst(ICmpInst::ICMP_ULT, Left, Right);
            case '>':
                return new ICmpInst(ICmpInst::ICMP_UGT, Left, Right);
            default:
                ErrorLogger::LogError("invalid binary operator");
                return nullptr;
        }
    }

    if (Left->getType()->isDoubleTy() || Left->getType()->isFloatTy()) {
        if (Right->getType() != Left->getType())
            Right = CodeGen::Builder->CreateUIToFP(Right, Left->getType(), "uitofp");


        switch (Op) {
            case '+':
                return BinaryOperator::CreateFAdd(Left, Right, "iaddtmp");
            case '-':
                return BinaryOperator::CreateFSub(Left, Right, "isubtmp");
            case '*':
                return BinaryOperator::CreateFMul(Left, Right, "imultmp");
            case '/':
                return BinaryOperator::CreateFDiv(Left, Right, "idivtmp");
            case '<':
                return new FCmpInst(FCmpInst::ICMP_ULT, Left, Right);
            case '>':
                // TODO: Convert bool 0/1 to double 0.0 or 1.0
                return new FCmpInst(FCmpInst::ICMP_UGT, Left, Right);
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

    // the type of LHS prevails
    auto FnIR = CreateOp(binExprAST->getOp(), Left, Right);

//    if (auto *FnIR = exprAST->codegen()) {
//      if (auto *FnIR = exprAST->Perform(codegen)) {
    if (FnIR) {
        fprintf(stderr, "Read top-level expression:");
        FnIR->print(errs());
        fprintf(stderr, "\n");

        // Remove the anonymous expression.
//        FnIR->eraseFromParent();
    } else ErrorLogger::LogError("No IR was generated!");
}

llvm::Value *CodeGen::GetResult() {
    return Result;
}

void CodeGen::On(std::shared_ptr<PrototypeAST> prototypeAST) {
    Result = nullptr;
}

void CodeGen::On(std::shared_ptr<FunctionAST> functionAST) {
    Result = nullptr;
}