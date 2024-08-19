#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "frontend/lexer.h"
#include "frontend/token.h"

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

    Token tkn = lexer.GetNextToken();
    while (tkn.kind != TokenKind::Eof)
    {
        std::cout << tkn << std::endl;
        tkn = lexer.GetNextToken();
    }
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
