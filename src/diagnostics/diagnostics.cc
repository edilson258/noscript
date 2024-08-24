#include "diagnostics.h"
#include <string>

void Diagnostics::EmitAll(std::ostream &os, std::string &fileName, std::string &fileContents)
{
    for (const DiagnosticError &err : m_Diagnostics)
    {
        const Location &loc = err.GetLocation();

        os << Painter::Paint(fileName, Color::Cyan) << ":" << loc.GetLine() << ":" << loc.GetColumn() << " ";
        os << Painter::Paint("ERROR", Color::RedBold) << " ";
        os << Painter::Paint(std::to_string(static_cast<int>(err.GetKind())).append(":"), Color::BrownLow) << " ";
        os << err.GetMessage() << std::endl << std::endl;
        os << Painter::HighlightRange(fileContents, loc.GetStart(), loc.GetEnd(), "\x1b[31m^\x1b[0m") << std::endl;
    }

    os << Painter::Paint(std::string("Done. ").append(std::to_string(m_Diagnostics.size()).append(" erros found")),
                         Color::BrownLow);
    os << std::endl;
}
