#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "checker.h"

void Checker::Check(const std::unique_ptr<Ast> &ast)
{
    for (const std::unique_ptr<Statement> &stmt : ast->block)
    {
        switch (stmt->Kind)
        {
        case StatementKind::VariableDeclaration:
            break;
        case StatementKind::Expression:
            checkStatementExpression(static_cast<const StatementExpression *>(stmt.get()));
            break;
        }
    }

    diagnostics.EmitAll(std::cerr, fileName, raw);
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
        return std::make_shared<Type>(TypeKind::Number);
    case LiteralKind::String:
        return std::make_shared<Type>(TypeKind::String);
    }
}

std::shared_ptr<Type> Checker::checkExpressionIdentifier(const ExpressionIdentifier *identifier)
{
    std::shared_ptr<Type> typeValue = scope.Resolve(identifier->Label);

    if (nullptr == typeValue)
    {
        emitErrorNameNotDefined(identifier->location, identifier->Label);
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
        emitErrorNoField(ma->Field->location, lhsType, ma->Field->Label);
        return nullptr;
    }

    const TypeObject *object = static_cast<const TypeObject *>(lhsType.get());

    std::shared_ptr<Type> field = object->GetField(ma->Field->Label);

    if (nullptr == field)
    {
        emitErrorNoField(ma->Field->location, lhsType, ma->Field->Label);
        return nullptr;
    }

    return field;
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
        emitErrorNotCallable(call->location, calleeType);
        return nullptr;
    }

    const TypeFunction *calleeFunction = static_cast<const TypeFunction *>(calleeType.get());
    const std::vector<std::shared_ptr<Type>> calleeFunctionParams = calleeFunction->GetParamaters();

    if (calleeFunctionParams.size() > call->Args.size() ||
        (!calleeFunction->GetIsVarArgs() && calleeFunctionParams.size() != call->Args.size()))
    {
        emitErrorArgsCountNoMatch(call->location, calleeFunctionParams.size(), call->Args.size());
        return nullptr;
    }

    for (const std::shared_ptr<Type> &paramType : calleeFunctionParams)
    {
        for (const std::unique_ptr<StatementExpression> &arg : call->Args)
        {
            std::shared_ptr<Type> argType = checkStatementExpression(arg.get());

            if (nullptr == argType)
            {
                // ignore and continue because the error was already reported down the tree
                continue;
            }

            if (!(*paramType.get() == *argType.get()))
            {
                emitErrorArgsTypesNoMatch(arg->location, paramType, argType);
                return nullptr;
            }
        }
    }

    return calleeFunction->GetReturnType();
}

void Checker::emitErrorNoField(const Location &location, std::shared_ptr<Type> object, std::string &field)
{
    std::ostringstream messageStream;
    messageStream << "Field `" << field << "` ";
    messageStream << "does not exit on type `" << *object.get() << "`.";
    diagnostics.RegisterError(DiagnosticError(ErrorKind::UnboundProperty, location, messageStream.str()));
}

void Checker::emitErrorNameNotDefined(const Location &location, const std::string &name)
{
    std::ostringstream messageStream;
    messageStream << "Cannot find name `" << name << "`.";
    diagnostics.RegisterError(DiagnosticError(ErrorKind::UnboundName, location, messageStream.str()));
}

void Checker::emitErrorArgsCountNoMatch(const Location &location, size_t expectedCount, size_t providedCount)
{
    std::ostringstream messageStream;
    messageStream << "Expected " << expectedCount << " arguments ";
    messageStream << "but got " << providedCount << ".";
    diagnostics.RegisterError(DiagnosticError(ErrorKind::ArgumentsCountNoMatch, location, messageStream.str()));
}

void Checker::emitErrorNotCallable(const Location &location, std::shared_ptr<Type> calleeType)
{
    std::ostringstream messageStream;
    messageStream << "This expression is not callable." << std::endl;
    messageStream << "  Type `" << *calleeType.get() << "` has no call signature.";
    diagnostics.RegisterError(DiagnosticError(ErrorKind::ArgumentsCountNoMatch, location, messageStream.str()));
}

void Checker::emitErrorArgsTypesNoMatch(const Location &location, std::shared_ptr<Type> expectedType,
                                        std::shared_ptr<Type> providedType)
{
    std::ostringstream messageStream;
    messageStream << "Argument of type `" << *providedType.get() << "`";
    messageStream << " is not assignable to parameter of type `" << *expectedType.get() << "`.";
    diagnostics.RegisterError(DiagnosticError(ErrorKind::ArgumentsTypesNoMatch, location, messageStream.str()));
}
