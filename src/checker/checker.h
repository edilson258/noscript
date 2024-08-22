#pragma once

#include <memory>
#include <string>

#include "../frontend/ast.h"
#include "../scope/scope.h"

class Checker
{
  private:
    Scope scope;
    std::string &raw;
    std::string &fileName;

    std::shared_ptr<Type> checkStatementExpression(const StatementExpression *);
    std::shared_ptr<Type> checkExpressionLiteral(const ExpressionLiteral *);
    std::shared_ptr<Type> checkExpressionIdentifier(const ExpressionIdentifier *);
    std::shared_ptr<Type> checkExpressionMemberAccess(const ExpressionMemberAccess *);
    std::shared_ptr<Type> checkExpressionFunctionCall(const ExpressionFunctionCall *);

  public:
    Checker(std::string &fileName, std::string &raw, std::unordered_map<std::string, std::shared_ptr<Type>> entries)
        : scope(entries), fileName(fileName), raw(raw) {};

    void Check(const std::unique_ptr<Ast> &);
};
