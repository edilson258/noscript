#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

enum class StatementKind
{
    Expression = 1
};

enum class ExpressionKind
{
    Literal = 1,
    Identifier = 2,
    FunctionCall = 3,
    MemberAccess = 4,
};

enum class LiteralKind
{
    String = 1,
    Number = 2
};

class Statement
{
  public:
    const StatementKind Kind;

    Statement(StatementKind kind) : Kind(kind) {};
    ~Statement() = default;
};

class StatementExpression : public Statement
{
  public:
    const ExpressionKind Kind;

    StatementExpression(ExpressionKind kind) : Statement(StatementKind::Expression), Kind(kind) {};
    ~StatementExpression() = default;
};

class ExpressionFunctionCall : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Callee;
    std::vector<std::unique_ptr<StatementExpression>> Args;

    ExpressionFunctionCall(std::unique_ptr<StatementExpression> callee,
                           std::vector<std::unique_ptr<StatementExpression>> args)
        : StatementExpression(ExpressionKind::FunctionCall), Callee(std::move(callee)), Args(std::move(args)) {};
    ~ExpressionFunctionCall() = default;
};

class ExpressionMemberAccess : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Object;
    std::unique_ptr<StatementExpression> Member;

    ExpressionMemberAccess(std::unique_ptr<StatementExpression> object, std::unique_ptr<StatementExpression> member)
        : StatementExpression(ExpressionKind::MemberAccess), Object(std::move(object)), Member(std::move(member)) {};
    ~ExpressionMemberAccess() = default;
};

class ExpressionIdentifier : public StatementExpression
{
  public:
    std::string Label;

    ExpressionIdentifier(std::string label) : StatementExpression(ExpressionKind::Identifier), Label(label) {};
    ~ExpressionIdentifier() = default;
};

class ExpressionLiteral : public StatementExpression
{
  public:
    const LiteralKind Kind;
    std::variant<std::monostate, std::string, long long> Value;

    ExpressionLiteral(std::string value)
        : StatementExpression(ExpressionKind::Literal), Kind(LiteralKind::String), Value(value) {};
    ExpressionLiteral(long long value)
        : StatementExpression(ExpressionKind::Literal), Kind(LiteralKind::Number), Value(value) {};
    ~ExpressionLiteral() = default;
};

class Ast
{
  public:
    std::vector<std::unique_ptr<Statement>> block;

    Ast() : block() {};
    ~Ast() = default;
};

std::ostream &operator<<(std::ostream &os, const Ast &);
