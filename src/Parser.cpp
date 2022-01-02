#include "llvm/Support/raw_ostream.h"
#include "Parser.hpp"

using namespace kal;
using namespace llvm;


/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// integerexpr ::= integer
std::unique_ptr<ExprAST> Parser::ParseIntegerExpr() {
  auto Result = std::make_unique<IntegerExprAST>(lexer.IntVal);
  currToken = lexer.GetToken(); // consume the integer
  return std::move(Result);
}

/// floatexpr ::= float
std::unique_ptr<ExprAST> Parser::ParseFloatExpr() {
  auto Result = std::make_unique<FloatExprAST>(lexer.IntVal);
  currToken = lexer.GetToken(); // consume the float
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
  currToken = lexer.GetToken();  // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (currToken != ')')
    return LogError("expected ')'");
  currToken = lexer.GetToken(); // eat ).
  return V;
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> Parser::ParsePrimary() {
  switch (currToken) {
  default:
    return LogError("unknown token when expecting an expression");
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


/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> Parser::ParseExpression() {
  // auto LHS = ParsePrimary();
  // if (!LHS)
  //   return nullptr;

  // return ParseBinOpRHS(0, std::move(LHS));

  return ParsePrimary();
}

void Parser::HandleTopLevelExpression() {
  // // Evaluate a top-level expression into an anonymous function.
  // if (ParseTopLevelExpr()) {

  if (ParseExpression()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    currToken = lexer.GetToken();
  }
}

/// top ::=  expression | ';'
int Parser::EatTokens(int token) {
  // currToken = token;
  switch (currToken = token) {
  case tok_eof:
    break;
  case ';': // ignore top-level semicolons.
    currToken = lexer.GetToken();
    break;
  default:
    HandleTopLevelExpression();
    break;
  }

  return currToken;
}
