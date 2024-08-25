#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../diagnostics/diagnostics.h"
#include "ast.h"
#include "lexer.h"
#include "token.h"

enum class Precedence
{
    Lowest = 1,
    Call = 2,
    MemberAccess = 3,
};

Precedence tokenToPrecedence(TokenKind);

class Parser
{
  private:
    Lexer &m_Lexer;
    Token m_CurrentToken;
    Token m_NextToken;

    void bump();
    void bumpExpect(TokenKind, Location, std::string);

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<StatementExpression> parseStatementExpression(Precedence);
    std::unique_ptr<ExpressionMemberAccess> parseExpressionMemberAccess(std::unique_ptr<StatementExpression>);
    std::unique_ptr<ExpressionFunctionCall> parseExpressionFunctionCall(std::unique_ptr<StatementExpression>);
    std::vector<std::unique_ptr<StatementExpression>> parseListOfExpressions(TokenKind);

    // helpers
    Precedence currTokenPrecedence();

    Diagnostics diagnostics;

  public:
    Parser(Lexer &lexer) : m_Lexer(lexer) {};
    std::unique_ptr<Ast> Parse();
};
