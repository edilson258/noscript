#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "checker/checker.h"
#include "frontend/ast.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "frontend/token.h"
#include "stdlib/stdlib.h"

std::string readFile(std::filesystem::path path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing source file" << std::endl;
        abort();
    }

    std::string filePath(argv[1]);
    std::string raw = readFile(filePath);

    Lexer lexer(filePath, raw);

    Parser parser(lexer);
    std::unique_ptr<Ast> ast = parser.Parse();

    std::cout << *ast.get();

    Checker checker(filePath, raw, StandardLibrary::GetBuiltins());
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
