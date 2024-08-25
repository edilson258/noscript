#include <memory>
#include <ostream>
#include <vector>

#include "ast.h"

class AstDumper
{
  private:
    int m_Tab;
    int m_TabRate;
    std::ostream &m_os;

    void tab() { m_Tab += m_TabRate; }
    void unTab() { m_Tab -= m_TabRate; }

    void _write_() {}
    template <typename T, typename... Args> void _write_(T first, Args... args)
    {
        m_os << first;
        _write_(args...);
    }

    void Write() {}
    template <typename T, typename... Args> void Write(T first, Args... args)
    {
        for (int i = 0; i < m_Tab; ++i)
        {
            m_os << " ";
        }
        _write_(first, args...);
    }

    void WriteLn() { m_os << std::endl; }
    template <typename T, typename... Args> void WriteLn(T first, Args... args)
    {
        Write(first, args...);
        m_os << std::endl;
    }

    void DumpSmtBlock(const std::vector<std::unique_ptr<Statement>> &);
    void DumpStmt(const Statement *);
    void DumpStmtExpr(const StatementExpression *);
    void DumpExprLiteral(const ExpressionLiteral *);
    void DumpExprIdentifier(const ExpressionIdentifier *);
    void DumpExprMemberAccess(const ExpressionMemberAccess *);
    void DumpExprFunctionCall(const ExpressionFunctionCall *);

  public:
    AstDumper(std::ostream &os, int tab) : m_os(os), m_TabRate(tab), m_Tab(0) {};

    void Dump(const std::vector<std::unique_ptr<Statement>> &);
};

std::ostream &operator<<(std::ostream &os, const Ast &ast)
{
    AstDumper dumper(os, 4);
    dumper.Dump(ast.block);
    return os;
}

void AstDumper::Dump(const std::vector<std::unique_ptr<Statement>> &block) { DumpSmtBlock(block); }

void AstDumper::DumpSmtBlock(const std::vector<std::unique_ptr<Statement>> &block)
{
    for (const std::unique_ptr<Statement> &stmt : block)
    {
        DumpStmt(stmt.get());
    }
}

void AstDumper::DumpStmt(const Statement *stmt)
{
    switch (stmt->Kind)
    {
    case StatementKind::Expression:
    {
        return DumpStmtExpr(static_cast<const StatementExpression *>(stmt));
    }
    }
}

void AstDumper::DumpStmtExpr(const StatementExpression *expr)
{
    WriteLn(expr->location.GetLine(), ":", expr->location.GetColumn(), "-", expr->location.GetStart(), ":",
            expr->location.GetEnd());
    switch (expr->Kind)
    {
    case ExpressionKind::Literal:
        return DumpExprLiteral(static_cast<const ExpressionLiteral *>(expr));
    case ExpressionKind::Identifier:
        return DumpExprIdentifier(static_cast<const ExpressionIdentifier *>(expr));
    case ExpressionKind::MemberAccess:
        return DumpExprMemberAccess(static_cast<const ExpressionMemberAccess *>(expr));
    case ExpressionKind::FunctionCall:
        return DumpExprFunctionCall(static_cast<const ExpressionFunctionCall *>(expr));
    }
}

void AstDumper::DumpExprLiteral(const ExpressionLiteral *literal)
{
    switch (literal->Kind)
    {
    case LiteralKind::Number:
        WriteLn("Literal(", std::get<long double>(literal->Value), ")");
        break;
    case LiteralKind::String:
        WriteLn("Literal(\"", std::get<std::string>(literal->Value), "\")");
        break;
    }
}

void AstDumper::DumpExprIdentifier(const ExpressionIdentifier *ident) { WriteLn("Identifier(", ident->Label, ")"); }

void AstDumper::DumpExprMemberAccess(const ExpressionMemberAccess *ma)
{
    WriteLn("Member Access Expression:");
    tab();
    WriteLn("Source:");
    tab();
    DumpStmtExpr(ma->Object.get());
    unTab();
    WriteLn("Member:");
    tab();
    DumpStmtExpr(ma->Field.get());
    unTab();
    unTab();
}

void AstDumper::DumpExprFunctionCall(const ExpressionFunctionCall *call)
{
    WriteLn("Function Call Expression:");
    tab();
    WriteLn("Callee:");
    tab();
    DumpStmtExpr(call->Callee.get());
    unTab();
    WriteLn("Args: [");
    tab();
    for (const std::unique_ptr<StatementExpression> &arg : call->Args)
    {
        DumpStmtExpr(arg.get());
    }
    unTab();
    WriteLn("]");
    unTab();
}
