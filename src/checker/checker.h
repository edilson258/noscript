#pragma once

#include <memory>
#include <string>

#include "../diagnostics/diagnostics.h"
#include "../frontend/ast.h"
#include "../scope/scope.h"

class Checker
{
  private:
    Scope scope;
    std::string &raw;
    std::string &fileName;

    Diagnostics diagnostics;

    std::shared_ptr<Type> checkStatementExpression(const StatementExpression *);
    std::shared_ptr<Type> checkExpressionLiteral(const ExpressionLiteral *);
    std::shared_ptr<Type> checkExpressionIdentifier(const ExpressionIdentifier *);
    std::shared_ptr<Type> checkExpressionMemberAccess(const ExpressionMemberAccess *);
    std::shared_ptr<Type> checkExpressionFunctionCall(const ExpressionFunctionCall *);

    void emitErrorNameNotDefined(Location &location, std::string &label);
    void emitErrorNotCallable(Location &location, std::shared_ptr<Type> calleeType);
    void emitErrorNoField(Location &location, std::shared_ptr<Type> object, std::string &label);
    void emitErrorArgsCountNoMatch(Location &location, size_t expectedCount, size_t providedCount);
    void emitErrorArgsTypesNoMatch(Location &location, std::shared_ptr<Type> expectedType,
                                   std::shared_ptr<Type> providedType);

  public:
    Checker(std::string &fileName, std::string &raw, std::unordered_map<std::string, std::shared_ptr<Type>> entries)
        : scope(entries), fileName(fileName), raw(raw) {};

    void Check(const std::unique_ptr<Ast> &);
};
