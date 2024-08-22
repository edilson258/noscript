#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "../range/range.h"

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
    Range range;

    StatementExpression(ExpressionKind kind, Range range)
        : Statement(StatementKind::Expression), Kind(kind), range(range) {};
    ~StatementExpression() = default;
};

class ExpressionFunctionCall : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Callee;
    std::vector<std::unique_ptr<StatementExpression>> Args;

    ExpressionFunctionCall(std::unique_ptr<StatementExpression> callee,
                           std::vector<std::unique_ptr<StatementExpression>> args, Range range)
        : StatementExpression(ExpressionKind::FunctionCall, range), Callee(std::move(callee)), Args(std::move(args)) {};
    ~ExpressionFunctionCall() = default;
};

class ExpressionIdentifier : public StatementExpression
{
  public:
    std::string Label;

    ExpressionIdentifier(std::string label, Range range)
        : StatementExpression(ExpressionKind::Identifier, range), Label(label) {};
    ~ExpressionIdentifier() = default;
};

class ExpressionMemberAccess : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Object;
    std::unique_ptr<ExpressionIdentifier> Field;

    ExpressionMemberAccess(std::unique_ptr<StatementExpression> object, std::unique_ptr<ExpressionIdentifier> field,
                           Range range)
        : StatementExpression(ExpressionKind::MemberAccess, range), Object(std::move(object)),
          Field(std::move(field)) {};
    ~ExpressionMemberAccess() = default;
};

class ExpressionLiteral : public StatementExpression
{
  public:
    const LiteralKind Kind;
    std::variant<std::monostate, std::string, long long> Value;

    ExpressionLiteral(std::string value, Range range)
        : StatementExpression(ExpressionKind::Literal, range), Kind(LiteralKind::String), Value(value) {};
    ExpressionLiteral(long long value, Range range)
        : StatementExpression(ExpressionKind::Literal, range), Kind(LiteralKind::Number), Value(value) {};
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
