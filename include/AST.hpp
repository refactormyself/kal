#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <llvm/IR/Value.h>
#include <utility>
#include <vector>

namespace kal {
    class Operation;

    /// ExprAST - Base class for all expression nodes.
    class ExprAST//: public std::enable_shared_from_this<ExprAST>
    {
    public:
        virtual ~ExprAST() = default;
        virtual void Perform(Operation &op) = 0;
    };

    /// IntegerExprAST - Expression class for integer literals like "1".
    class IntegerExprAST : public ExprAST, public std::enable_shared_from_this<IntegerExprAST>
    {
        int val;

    public:
        explicit IntegerExprAST(int val) : val(val) {}
        int GetVal() const { return val; };
        void Perform(Operation &op) override;
    };

    /// FloatExprAST - Expression class for floating point literals like "1.0".
    class FloatExprAST : public ExprAST, public std::enable_shared_from_this<FloatExprAST>
    {
        double Val;

    public:
        explicit FloatExprAST(double Val) : Val(Val) {}
        void Perform(Operation &op) override;
        double GetVal() const;
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
    class UnaryExprAST : public ExprAST, public std::enable_shared_from_this<UnaryExprAST>
    {
        char Opcode;
        std::unique_ptr<ExprAST> Operand;

    public:
        UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}
        void Perform(Operation &op) override;
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST, public std::enable_shared_from_this<BinaryExprAST>
    {
        char Op;
        std::shared_ptr<ExprAST> LHS, RHS;
    public:
        BinaryExprAST(char Op, std::shared_ptr<ExprAST> LHS,
                      std::shared_ptr<ExprAST> RHS)
                : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        char getOp() const;
        const std::shared_ptr<ExprAST> &getLHS() const;
        const std::shared_ptr<ExprAST> &getRHS() const;
        void Perform(Operation &op) override;
    };

//    TODO: Implement typed functions, arguments, variables
/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
    class PrototypeAST : public ExprAST, public std::enable_shared_from_this<PrototypeAST> {
        std::string Name;
        std::vector<std::string> Args;

    public:
        PrototypeAST(std::string Name, std::vector<std::string> Args)
                : Name(std::move(Name)), Args(std::move(Args)) {}

        void Perform(Operation &op) override;
        const std::string &getName() const { return Name; }
        const std::vector<std::string> &getArgs() const;
    };

/// FunctionAST - This class represents a function definition itself.
    class FunctionAST : public ExprAST, public std::enable_shared_from_this<FunctionAST>{
        std::shared_ptr<PrototypeAST> Proto;
        std::shared_ptr<ExprAST> Body;

    public:
        FunctionAST(std::shared_ptr<PrototypeAST> Proto,
                    std::shared_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}

        void Perform(Operation &op) override;
        const std::shared_ptr<PrototypeAST> &getPrototype() const;
        const std::shared_ptr<ExprAST> &getBody() const;
    };

    class Operation
    {
    public:
        virtual void On(std::shared_ptr<IntegerExprAST> intExprAST) = 0;
        virtual void On(std::shared_ptr<FloatExprAST> floatExprAST) = 0;
        virtual void On(std::shared_ptr<UnaryExprAST> uniExprAST) = 0;
        virtual void On(std::shared_ptr<BinaryExprAST> binExprAST) = 0;
        virtual void On(std::shared_ptr<PrototypeAST> prototypeAST) = 0;
        virtual void On(std::shared_ptr<FunctionAST> functionAST) = 0;
    };
}
#endif //AST_HPP