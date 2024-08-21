#pragma once

#include <memory>

#include "../frontend/ast.h"
#include "../scope/scope.h"

class Checker
{
  private:
    Scope scope;

    std::shared_ptr<Type> checkStatementExpression(const StatementExpression *);
    std::shared_ptr<Type> checkExpressionLiteral(const ExpressionLiteral *);
    std::shared_ptr<Type> checkExpressionIdentifier(const ExpressionIdentifier *);
    std::shared_ptr<Type> checkExpressionMemberAccess(const ExpressionMemberAccess *);
    std::shared_ptr<Type> checkExpressionFunctionCall(const ExpressionFunctionCall *);

  public:
    Checker(std::unordered_map<std::string, std::shared_ptr<Type>> entries) : scope(entries) {};

    void Check(const std::unique_ptr<Ast> &);
};
