#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "../location/location.h"
#include "token.h"

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
    const Location location;

    Statement(StatementKind kind, Location location) : Kind(kind), location(location) {};
    ~Statement() = default;
};

class StatementExpression : public Statement
{
  public:
    const ExpressionKind Kind;

    StatementExpression(ExpressionKind kind, Location location)
        : Statement(StatementKind::Expression, location), Kind(kind) {};

    ~StatementExpression() = default;
};

class ExpressionFunctionCall : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Callee;
    std::vector<std::unique_ptr<StatementExpression>> Args;

    ExpressionFunctionCall(std::unique_ptr<StatementExpression> callee,
                           std::vector<std::unique_ptr<StatementExpression>> args, Location location)
        : StatementExpression(ExpressionKind::FunctionCall, location), Callee(std::move(callee)),
          Args(std::move(args)) {};
    ~ExpressionFunctionCall() = default;
};

class ExpressionIdentifier : public StatementExpression
{
  public:
    std::string Label;

    ExpressionIdentifier(std::string label, Location location)
        : StatementExpression(ExpressionKind::Identifier, location), Label(label) {};
    ~ExpressionIdentifier() = default;
};

class ExpressionMemberAccess : public StatementExpression
{
  public:
    std::unique_ptr<StatementExpression> Object;
    std::unique_ptr<ExpressionIdentifier> Field;

    ExpressionMemberAccess(std::unique_ptr<StatementExpression> object, std::unique_ptr<ExpressionIdentifier> field,
                           Location location)
        : StatementExpression(ExpressionKind::MemberAccess, location), Object(std::move(object)),
          Field(std::move(field)) {};
    ~ExpressionMemberAccess() = default;
};

class ExpressionLiteral : public StatementExpression
{
  public:
    const LiteralKind Kind;
    std::variant<std::monostate, std::string, long double> Value;

    ExpressionLiteral(std::string value, Location location)
        : StatementExpression(ExpressionKind::Literal, location), Kind(LiteralKind::String), Value(value) {};
    ExpressionLiteral(long double value, Location location)
        : StatementExpression(ExpressionKind::Literal, location), Kind(LiteralKind::Number), Value(value) {};
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
