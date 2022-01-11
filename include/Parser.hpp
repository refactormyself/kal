#ifndef PARSER_HPP
#define PARSER_HPP

#include "AST.hpp"
#include "Lexer.hpp"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"

#include <map>
#include <memory>

namespace kal {
  class Parser
  {
  private:
    Lexer lexer;
    int currToken = 0;

    std::shared_ptr<ExprAST> ParseIntegerExpr();
    std::shared_ptr<ExprAST> ParseFloatExpr();
    std::shared_ptr<ExprAST> ParseParenExpr();
    std::shared_ptr<ExprAST> ParsePrimary();
    std::shared_ptr<ExprAST> ParseExpression();
    std::shared_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::shared_ptr<ExprAST> LHS);
      std::shared_ptr<FunctionAST> ParseTopLevelExpr();
      std::shared_ptr<FunctionAST> ParseDefinition();
      std::shared_ptr<PrototypeAST> ParsePrototype();
      std::shared_ptr<PrototypeAST> ParseExtern();
    void HandleTopLevelExpression();
    int GetTokenPrecedence() const;

  public:
    explicit Parser(const std::string &filename) : lexer(filename){}
    Parser() = default;

    int EatToken();
    static std::map<char, int> BinopPrecedence;


      void HandleExtern();
      void HandleDefinition();

      std::shared_ptr<ExprAST> ParseIdentifierExpr();
  };
}
#endif // PARSER_HPP