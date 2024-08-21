#include <fstream>
#include <sstream>

#include "checker/checker.h"
#include "frontend/ast.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "stdlib/stdlib.h"

std::string readFile(std::filesystem::path path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing source file" << std::endl;
        abort();
    }

    std::filesystem::path path(argv[1]);
    std::string raw = readFile(path);

    Lexer lexer(path, raw);
    Parser parser(lexer);
    std::unique_ptr<Ast> ast = parser.Parse();

    std::cout << *ast.get();

    Checker checker(StandardLibrary::GetBuiltins());
    checker.Check(ast);
}

std::string readFile(std::filesystem::path path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Coudn't open source file" << std::endl;
        abort();
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}
