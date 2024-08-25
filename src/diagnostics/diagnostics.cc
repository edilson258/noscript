#include <ostream>
#include <string>

#include "diagnostics.h"

void Diagnostics::displayError(std::ostream &os, const std::string &fileName, const std::string &fileContents,
                               DiagnosticError err)
{
    const Location &loc = err.GetLocation();
    os << Painter::Paint(fileName, Color::Cyan) << ":" << loc.GetLine() << ":" << loc.GetColumn() << " ";
    os << Painter::Paint("ERROR", Color::RedBold) << " ";
    os << Painter::Paint(std::to_string(static_cast<int>(err.GetKind())).append(":"), Color::BrownLow) << " ";
    os << err.GetMessage() << std::endl << std::endl;
    os << Painter::HighlightRange(fileContents, loc.GetStart(), loc.GetEnd(), "\x1b[31m~\x1b[0m") << std::endl;
}

void Diagnostics::EmitAll(std::ostream &os, const std::string &fileName, const std::string &fileContents)
{
    for (const DiagnosticError &err : m_Diagnostics)
    {
        displayError(os, fileName, fileContents, err);
    }

    os << Painter::Paint(std::string("Done. ").append(std::to_string(m_Diagnostics.size()).append(" erros found")),
                         Color::BrownLow);
    os << std::endl;
}

void Diagnostics::EmitNow(std::ostream &os, const std::string &fileName, const std::string &fileContents,
                          DiagnosticError err)
{
    displayError(os, fileName, fileContents, err);
}
