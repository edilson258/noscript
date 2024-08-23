#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "ast.h"
#include "parser.h"
#include "token.h"

std::unique_ptr<Ast> Parser::Parse()
{
    bump();
    bump();

    auto ast = std::make_unique<Ast>();

    while (TokenKind::Eof != m_CurrentToken.GetKind())
    {
        ast.get()->block.push_back(std::move(parseStatement()));
    }

    return ast;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    switch (m_CurrentToken.GetKind())
    {
    default:
    {
        auto expression = parseStatementExpression(Precedence::Lowest);
        bumpExpect(TokenKind::Semicolon);
        return static_cast<std::unique_ptr<Statement>>(std::move(expression));
    }
    }
}

std::unique_ptr<StatementExpression> Parser::parseStatementExpression(Precedence precedence)
{
    std::unique_ptr<StatementExpression> lhs;

    switch (m_CurrentToken.GetKind())
    {
    case TokenKind::String:
        lhs = std::make_unique<ExpressionLiteral>(std::get<std::string>(m_CurrentToken.GetData()),
                                                  m_CurrentToken.GetLocation());

        break;
    case TokenKind::Identifier:
        lhs = std::make_unique<ExpressionIdentifier>(std::get<std::string>(m_CurrentToken.GetData()),
                                                     m_CurrentToken.GetLocation());
        break;
    default:
        std::cerr << "[Error] unexpected lhs expression: " << m_CurrentToken << std::endl;
        abort();
    }

    bump();

    while (TokenKind::Eof != m_CurrentToken.GetKind() && precedence < currTokenPrecedence())
    {
        switch (m_CurrentToken.GetKind())
        {
        case TokenKind::Dot:
            lhs = parseExpressionMemberAccess(std::move(lhs));
            break;
        case TokenKind::LeftParent:
            lhs = parseExpressionFunctionCall(std::move(lhs));
            break;
        default:
            return lhs;
        }
    }

    return lhs;
}

std::unique_ptr<ExpressionMemberAccess> Parser::parseExpressionMemberAccess(std::unique_ptr<StatementExpression> object)
{
    bumpExpect(TokenKind::Dot);
    Location location = (Location)object->location + m_CurrentToken.GetLocation();
    auto expression = parseStatementExpression(Precedence::MemberAccess);

    if (ExpressionKind::Identifier != expression.get()->Kind)
    {
        std::cerr << "rhs of '.' must be an identifier" << std::endl;
        abort();
    }

    auto tmp = static_cast<ExpressionIdentifier *>(expression.get());
    auto fieldIdentifier = std::make_unique<ExpressionIdentifier>(tmp->Label, tmp->location);

    return std::make_unique<ExpressionMemberAccess>(std::move(object), std::move(fieldIdentifier), location);
}

std::unique_ptr<ExpressionFunctionCall> Parser::parseExpressionFunctionCall(std::unique_ptr<StatementExpression> callee)
{
    bumpExpect(TokenKind::LeftParent);
    std::vector<std::unique_ptr<StatementExpression>> args = parseListOfExpressions(TokenKind::RightParent);
    auto range = (Location)callee->location + m_CurrentToken.GetLocation();
    bumpExpect(TokenKind::RightParent);
    return std::make_unique<ExpressionFunctionCall>(std::move(callee), std::move(args), range);
}

std::vector<std::unique_ptr<StatementExpression>> Parser::parseListOfExpressions(TokenKind stop)
{
    std::vector<std::unique_ptr<StatementExpression>> xs;

    while (m_CurrentToken.GetKind() != stop)
    {
        xs.push_back(parseStatementExpression(Precedence::Lowest));
    }

    return xs;
}

Precedence Parser::currTokenPrecedence() { return tokenToPrecedence(m_CurrentToken.GetKind()); }

Precedence tokenToPrecedence(TokenKind tokenKind)
{
    switch (tokenKind)
    {
    case TokenKind::Dot:
        return Precedence::MemberAccess;
    case TokenKind::LeftParent:
        return Precedence::Call;
    default:
        return Precedence::Lowest;
    }
}

void Parser::bump()
{
    m_CurrentToken = m_NextToken;
    m_NextToken = m_Lexer.GetNextToken();
}

void Parser::bumpExpect(TokenKind expected_kind)
{
    if (expected_kind != m_CurrentToken.GetKind())
    {
        std::cerr << "Error: expected token " << expected_kind;
        std::cerr << " but got " << m_CurrentToken.GetKind();
        abort();
    }
    bump();
}
