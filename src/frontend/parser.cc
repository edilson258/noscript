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

    while (TokenKind::Eof != m_CurrentToken.kind)
    {
        ast.get()->block.push_back(std::move(parseStatement()));
    }

    return ast;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    switch (m_CurrentToken.kind)
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

    switch (m_CurrentToken.kind)
    {
    case TokenKind::String:
        lhs = std::make_unique<ExpressionLiteral>(std::get<std::string>(m_CurrentToken.data));
        break;
    case TokenKind::Identifier:
        lhs = std::make_unique<ExpressionIdentifier>(std::get<std::string>(m_CurrentToken.data));
        break;
    default:
        std::cerr << "[Error] unexpected lhs expression: " << m_CurrentToken << std::endl;
        abort();
    }

    bump();

    while (TokenKind::Eof != m_CurrentToken.kind && precedence < currTokenPrecedence())
    {
        switch (m_CurrentToken.kind)
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
    auto member = parseStatementExpression(Precedence::MemberAccess);
    return std::make_unique<ExpressionMemberAccess>(std::move(object), std::move(member));
}

std::unique_ptr<ExpressionFunctionCall> Parser::parseExpressionFunctionCall(std::unique_ptr<StatementExpression> callee)
{
    bumpExpect(TokenKind::LeftParent);
    std::vector<std::unique_ptr<StatementExpression>> args = parseListOfExpressions(TokenKind::RightParent);
    bumpExpect(TokenKind::RightParent);
    return std::make_unique<ExpressionFunctionCall>(std::move(callee), std::move(args));
}

std::vector<std::unique_ptr<StatementExpression>> Parser::parseListOfExpressions(TokenKind stop)
{
    std::vector<std::unique_ptr<StatementExpression>> xs;

    while (m_CurrentToken.kind != stop)
    {
        xs.push_back(parseStatementExpression(Precedence::Lowest));
    }

    return xs;
}

Precedence Parser::currTokenPrecedence() { return tokenToPrecedence(m_CurrentToken.kind); }

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
    if (expected_kind != m_CurrentToken.kind)
    {
        std::cerr << "Error: expected token " << expected_kind;
        std::cerr << " but got " << m_CurrentToken.kind;
        std::cerr << " at " << m_CurrentToken.pos.line << ":" << m_CurrentToken.pos.column << std::endl;
        abort();
    }
    bump();
}
