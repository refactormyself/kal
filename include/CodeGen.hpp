#ifndef KAL_COMPILER_CODEGEN_HPP
#define KAL_COMPILER_CODEGEN_HPP

#include <map>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Constant.h"
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
        std::map<std::string, llvm::Value *> NamedValues;
        llvm::Value *Result = nullptr;

    public:
        void On(std::shared_ptr<IntegerExprAST>intExprAST) override;
        void On(std::shared_ptr<FloatExprAST> floatExprAST) override;
        void On(std::shared_ptr<UnaryExprAST> uniExprAST) override;
        void On(std::shared_ptr<BinaryExprAST> binExprAST) override;
        void On(std::shared_ptr<PrototypeAST> prototypeAST) override;
        llvm::Value *GetResult();
        static std::unique_ptr<llvm::LLVMContext> TheContext;
        static std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> Builder;
        static std::unique_ptr<llvm::Module> TheModule;
    };
}

#endif //KAL_COMPILER_CODEGEN_HPP