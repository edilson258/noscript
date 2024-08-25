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
    const std::string &raw;
    const std::string &fileName;

    Diagnostics diagnostics;

    std::shared_ptr<Type> checkStatementExpression(const StatementExpression *);
    std::shared_ptr<Type> checkExpressionLiteral(const ExpressionLiteral *);
    std::shared_ptr<Type> checkExpressionIdentifier(const ExpressionIdentifier *);
    std::shared_ptr<Type> checkExpressionMemberAccess(const ExpressionMemberAccess *);
    std::shared_ptr<Type> checkExpressionFunctionCall(const ExpressionFunctionCall *);

    void emitErrorNameNotDefined(const Location &location, const std::string &label);
    void emitErrorNotCallable(const Location &location, std::shared_ptr<Type> calleeType);
    void emitErrorNoField(const Location &location, std::shared_ptr<Type> object, std::string &label);
    void emitErrorArgsCountNoMatch(const Location &location, size_t expectedCount, size_t providedCount);
    void emitErrorArgsTypesNoMatch(const Location &location, std::shared_ptr<Type> expectedType,
                                   std::shared_ptr<Type> providedType);

  public:
    Checker(const std::string &fileName, const std::string &raw,
            std::unordered_map<std::string, std::shared_ptr<Type>> entries)
        : scope(entries), raw(raw), fileName(fileName) {};

    void Check(const std::unique_ptr<Ast> &);
};
