#ifndef PARSER_HPP
#define PARSER_HPP

#include "AST.hpp"
#include "Lexer.hpp"

#include <map>
#include <memory>

namespace kal {
  class Parser
  {
  private:
    Lexer &lexer;
    int currToken;

    std::unique_ptr<ExprAST> ParseIntegerExpr();
    std::unique_ptr<ExprAST> ParseFloatExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseExpression();

    void HandleTopLevelExpression();

  public:
    Parser(Lexer &lex) : lexer(lex) {}
    int EatTokens(int token);
  };
}
#endif // PARSER_HPP