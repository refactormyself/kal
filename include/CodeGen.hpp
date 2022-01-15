#ifndef KAL_COMPILER_CODEGEN_HPP
#define KAL_COMPILER_CODEGEN_HPP

#include <map>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/NoFolder.h>

#include "Parser.hpp"
#include "AST.hpp"

namespace kal {
    class ErrorLogger {
    public:
        /// LogError* - These are little helper functions for error handling.
        static std::unique_ptr<ExprAST> LogError(const char *Str) {
            fprintf(stderr, "Error: %s\n", Str);
            return nullptr;
        }
    };

    class CodeGen : public Operation {
    private:
        llvm::Value *valueResult = nullptr;
        llvm::Function *functionResult = nullptr;

    public:
        void On(std::shared_ptr<IntegerExprAST>intExprAST) override;
        void On(std::shared_ptr<FloatExprAST> floatExprAST) override;
        void On(std::shared_ptr<UnaryExprAST> uniExprAST) override;
        void On(std::shared_ptr<BinaryExprAST> binExprAST) override;
        void On(std::shared_ptr<PrototypeAST> prototypeAST) override;
        void On(std::shared_ptr<FunctionAST> functionAST) override;
        void On(std::shared_ptr<VariableExprAST> variableExprAST) override;
        void On(std::shared_ptr<CallExprAST> callExprAST) override;

        llvm::Value *TakeValueResult();
        llvm::Function *TakeFunctionResult();
        static std::unique_ptr<llvm::LLVMContext> TheContext;
        static std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> Builder;
        static std::unique_ptr<llvm::Module> TheModule;
        static std::map<std::string, llvm::AllocaInst *> NamedValues; // AllocaInst gives the variable's memory location
        void SetFunction(llvm::Function *pFunction);
    };
}

#endif //KAL_COMPILER_CODEGEN_HPP