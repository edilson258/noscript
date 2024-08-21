#include <iostream>
#include <memory>

#include "checker.h"

void Checker::Check(const std::unique_ptr<Ast> &ast)
{
    for (const std::unique_ptr<Statement> &stmt : ast.get()->block)
    {
        switch (stmt.get()->Kind)
        {
        case StatementKind::Expression:
            checkStatementExpression(static_cast<const StatementExpression *>(stmt.get()));
            break;
        }
    }
}

std::shared_ptr<Type> Checker::checkStatementExpression(const StatementExpression *expression)
{
    switch (expression->Kind)
    {
    case ExpressionKind::Literal:
        return checkExpressionLiteral(static_cast<const ExpressionLiteral *>(expression));
    case ExpressionKind::Identifier:
        return checkExpressionIdentifier(static_cast<const ExpressionIdentifier *>(expression));
    case ExpressionKind::MemberAccess:
        return checkExpressionMemberAccess(static_cast<const ExpressionMemberAccess *>(expression));
    case ExpressionKind::FunctionCall:
        return checkExpressionFunctionCall(static_cast<const ExpressionFunctionCall *>(expression));
    }
}

std::shared_ptr<Type> Checker::checkExpressionLiteral(const ExpressionLiteral *literal)
{
    switch (literal->Kind)
    {
    case LiteralKind::Number:
        return std::make_shared<Type>(TypeKind::Integer);
    case LiteralKind::String:
        return std::make_shared<Type>(TypeKind::String);
    }
}

std::shared_ptr<Type> Checker::checkExpressionIdentifier(const ExpressionIdentifier *identifier)
{
    std::shared_ptr<Type> typeValue = scope.Resolve(identifier->Label);

    if (nullptr == typeValue)
    {
        std::cerr << "[Error] " << identifier->Label << " is undefined" << std::endl;
        return nullptr;
    }

    return typeValue;
}

std::shared_ptr<Type> Checker::checkExpressionMemberAccess(const ExpressionMemberAccess *ma)
{
    std::shared_ptr<Type> lhsType = checkStatementExpression(ma->Object.get());

    if (nullptr == lhsType)
    {
        return nullptr;
    }

    if (TypeKind::Object != lhsType->GetKind())
    {
        std::cerr << "[Error] lhs of '.' operator must be an object" << std::endl;
        return nullptr;
    }

    if (ExpressionKind::Identifier != ma->Member.get()->Kind)
    {
        std::cerr << "[Error] rhs of '.' operator must be an identifier" << std::endl;
        return nullptr;
    }

    const TypeObject *object = static_cast<const TypeObject *>(lhsType.get());
    const ExpressionIdentifier *identifier = static_cast<const ExpressionIdentifier *>(ma->Member.get());

    std::shared_ptr<Type> accessProperty = object->GetField(identifier->Label);

    if (nullptr == accessProperty)
    {
        std::cerr << "[Error] object has not propery " << identifier->Label << std::endl;
        return nullptr;
    }

    return accessProperty;
}

std::shared_ptr<Type> Checker::checkExpressionFunctionCall(const ExpressionFunctionCall *callExpression)
{
    std::shared_ptr<Type> calleeType = checkStatementExpression(callExpression->Callee.get());

    if (nullptr == calleeType)
    {
        return nullptr;
    }

    if (TypeKind::Function != calleeType->GetKind())
    {
        std::cerr << "[Error] object is not callable" << std::endl;
        return nullptr;
    }

    const TypeFunction *calleeFunction = static_cast<const TypeFunction *>(calleeType.get());

    if (calleeFunction->Parameters.size() > callExpression->Args.size() ||
        (!calleeFunction->IsVarArgs && calleeFunction->Parameters.size() != callExpression->Args.size()))
    {
        std::cerr << "[Error] args count mismatch" << std::endl;
        return nullptr;
    }

    return calleeFunction->ReturnType;
}
