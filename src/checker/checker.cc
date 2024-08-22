#include <iostream>
#include <memory>
#include <string>

#include "../utils.h"
#include "checker.h"

std::string errorMessagePrefix() { return "\033[1m\x1b[31mERROR\x1b[0m:"; }

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
        std::cerr << errorMessagePrefix();
        std::cerr << " Cannot find name '\x1b[4m\x1b[31m" << identifier->Label << "\x1b[0m'." << std::endl;
        std::cerr << std::endl << highlightError(raw, identifier->range.Start, identifier->range.End) << std::endl;
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
        std::cerr << errorMessagePrefix();
        std::cerr << " Property \"\x1b[4m\x1b[31m" << ma->Field.get()->Label << "\x1b[0m\"";
        std::cerr << " does not exit on type '" << lhsType.get()->ToString() << "'." << std::endl;
        std::cerr << std::endl << highlightError(raw, ma->Field->range.Start, ma->Field->range.End) << std::endl;
        return nullptr;
    }

    const TypeObject *object = static_cast<const TypeObject *>(lhsType.get());

    std::shared_ptr<Type> accessProperty = object->GetField(ma->Field.get()->Label);

    if (nullptr == accessProperty)
    {
        std::cerr << errorMessagePrefix();
        std::cerr << " Property \"\x1b[4m\x1b[31m" << ma->Field.get()->Label << "\x1b[0m\"";
        std::cerr << " does not exit on type '" << object->ToString() << "'." << std::endl;
        std::cerr << std::endl << highlightError(raw, ma->Field->range.Start, ma->Field->range.End) << std::endl;
        return nullptr;
    }

    return accessProperty;
}

std::shared_ptr<Type> Checker::checkExpressionFunctionCall(const ExpressionFunctionCall *call)
{
    std::shared_ptr<Type> calleeType = checkStatementExpression(call->Callee.get());

    if (nullptr == calleeType)
    {
        return nullptr;
    }

    if (TypeKind::Function != calleeType->GetKind())
    {
        std::cerr << errorMessagePrefix();
        std::cerr << " This expression is not callable." << std::endl;
        std::cerr << " Type '" << calleeType->ToString() << "' has no call signature" << std::endl;
        std::cerr << std::endl << highlightError(raw, call->range.Start, call->range.End) << std::endl;
        return nullptr;
    }

    const TypeFunction *calleeFunction = static_cast<const TypeFunction *>(calleeType.get());

    if (calleeFunction->Parameters.size() > call->Args.size() ||
        (!calleeFunction->IsVarArgs && calleeFunction->Parameters.size() != call->Args.size()))
    {
        std::cerr << errorMessagePrefix();
        std::cerr << " Expected " << calleeFunction->Parameters.size() << " arguments";
        std::cerr << ", but got  " << call->Args.size() << std::endl;
        std::cerr << std::endl << highlightError(raw, call->range.Start, call->range.End) << std::endl;
        return nullptr;
    }

    return calleeFunction->ReturnType;
}
