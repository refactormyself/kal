#ifndef AST_HPP
#define AST_HPP

#include <memory>

namespace
{
    class Operation
    {
    public:
    
    };

    /// ExprAST - Base class for all expression nodes.
    class ExprAST
    {
    public:
        virtual ~ExprAST() = default;
        // virtual void perform(Operation &op) = 0;
        //   virtual Value *codegen() = 0;
    };

    /// IntegerExprAST - Expression class for integer literals like "1".
    class IntegerExprAST : public ExprAST
    {
        int val;

    public:
        IntegerExprAST(int val) : val(val) {}
        // void perform(Operation &op) override;

        //   Value *codegen() override;
    };

    /// FloatExprAST - Expression class for floating point literals like "1.0".
    class FloatExprAST : public ExprAST
    {
        double Val;

    public:
        FloatExprAST(double Val) : Val(Val) {}
        // void perform(Operation &op) override;

        //   Value *codegen() override;
    };

    // /// VariableExprAST - Expression class for referencing a variable, like "a".
    // class VariableExprAST : public ExprAST {
    //   std::string Name;

    // public:
    //   VariableExprAST(const std::string &Name) : Name(Name) {}

    //   Value *codegen() override;
    //   const std::string &getName() const { return Name; }
    // };

    /// UnaryExprAST - Expression class for a unary operator.
    class UnaryExprAST : public ExprAST
    {
        char Opcode;
        std::unique_ptr<ExprAST> Operand;

    public:
        UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}
        // void perform(Operation &op) override;

        //   Value *codegen() override;
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST
    {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

        // void perform(Operation &op) override;

        //   Value *codegen() override;
    };

}
#endif //AST_HPP