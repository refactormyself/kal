#include "AST.hpp"

using namespace  kal;

void IntegerExprAST::Perform(Operation &op) {
    std::shared_ptr<IntegerExprAST> ast{shared_from_this()};
    return op.On(ast);
}

void FloatExprAST::Perform(Operation &op) {
    std::shared_ptr<FloatExprAST> ast{shared_from_this()};
    return op.On(ast);
}

void BinaryExprAST::Perform(Operation &op) {
    std::shared_ptr<BinaryExprAST> ast{shared_from_this()};
    return op.On(ast);
}

void UnaryExprAST::Perform(Operation &op) {
    std::shared_ptr<UnaryExprAST> ast{shared_from_this()};
    return op.On(ast);
}

void PrototypeAST::Perform(Operation &op) {
    std::shared_ptr<PrototypeAST> ast{shared_from_this()};
    return op.On(ast);
}

double FloatExprAST::GetVal() const {
    return Val;
}

char BinaryExprAST::getOp() const {
    return Op;
}

const std::shared_ptr<ExprAST> &BinaryExprAST::getLHS() const {
    return LHS;
}

const std::shared_ptr<ExprAST> &BinaryExprAST::getRHS() const {
    return RHS;
}

const std::vector<std::string> &PrototypeAST::getArgs() const {
    return Args;
}
