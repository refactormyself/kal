#include "llvm/Support/raw_ostream.h"
#include "Parser.hpp"
#include "CodeGen.hpp"

using namespace kal;
using namespace llvm;

std::unique_ptr<ExprAST> ParseExpression();

/// integerexpr ::= integer
std::shared_ptr<ExprAST> Parser::ParseIntegerExpr() {
  auto Result = std::make_shared<IntegerExprAST>(lexer.IntVal);
  currToken = lexer.GetToken(); // consume the integer
  return std::move(Result);
}

/// floatexpr ::= float
std::shared_ptr<ExprAST> Parser::ParseFloatExpr() {
  auto Result = std::make_unique<FloatExprAST>(lexer.FloatVal);
  currToken = lexer.GetToken(); // consume the float
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
std::shared_ptr<ExprAST> Parser::ParseParenExpr() {
  currToken = lexer.GetToken();  // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (currToken != ')')
    return ErrorLogger::LogError("expected ')'");
  currToken = lexer.GetToken(); // eat ).
  return V;
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::shared_ptr<ExprAST> Parser::ParsePrimary() {
  switch (currToken) {
  default:
    return ErrorLogger::LogError("unknown token when expecting an expression");
  // case tok_identifier:
  //   return ParseIdentifierExpr();
  case tok_integer:
    return ParseIntegerExpr();
  case tok_float:
    return ParseFloatExpr();
  case '(':
    return ParseParenExpr();
  }
}

/// binop_rhs
///   ::= ('+' primary)*
std::shared_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                               std::shared_ptr<ExprAST> LHS) {
  while (true) {
    int TokPrec = GetTokenPrecedence();
    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = currToken;
    currToken = lexer.GetToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokenPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS = std::make_shared<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

/// expression
///   ::= primary binoprhs
///
std::shared_ptr<ExprAST> Parser::ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}


/// toplevelexpr ::= expression
std::shared_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = std::make_shared<PrototypeAST>("main",
                                                    std::vector<std::string>());
        return std::make_shared<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void Parser::HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto exprAST = ParseTopLevelExpr()) {
    CodeGen codegen{};

    exprAST->Perform(codegen);
//  auto *FnIR = exprAST->Perform(codegen);
//
////    if (auto *FnIR = exprAST->codegen()) {
////      if (auto *FnIR = exprAST->Perform(codegen)) {
//      if (FnIR) {
//        fprintf(stderr, "Read top-level expression:");
//        FnIR->print(errs());
//        fprintf(stderr, "\n");
//
//        // Remove the anonymous expression.
////        FnIR->eraseFromParent();
//    }
  } else {
    // Skip token for error recovery.
    currToken = lexer.GetToken();
  }
}

/// top ::=  expression | ';'
int Parser::EatToken() {
  switch (currToken = lexer.GetToken()) {
  case tok_eof:
  case tok_carr_ret:
  case ';': // ignore top-level semicolons.
    break;
  default:
    HandleTopLevelExpression();
    break;
  }

  return currToken;
}

std::map<char, int> Parser::BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
int Parser::GetTokenPrecedence() const {
  if (!isascii(currToken))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = Parser::BinopPrecedence[currToken];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}