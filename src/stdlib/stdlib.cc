#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "stdlib.h"

std::shared_ptr<Type> StandardLibrary::makeConsole()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> consoleFields;

    // log function
    bool isVarArgs = true;
    auto logFunctionReturnType = std::make_shared<Type>(TypeKind::Void);
    std::vector<std::unique_ptr<Type>> logFunctionParams{};
    auto logFunction = std::make_shared<TypeFunction>(isVarArgs, logFunctionReturnType, std::move(logFunctionParams));
    consoleFields["log"] = logFunction;

    return std::make_shared<TypeObject>(consoleFields);
}

std::unordered_map<std::string, std::shared_ptr<Type>> StandardLibrary::GetBuiltins()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> builtins;
    builtins["console"] = makeConsole();
    return builtins;
}
