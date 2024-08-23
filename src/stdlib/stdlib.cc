#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "stdlib.h"

std::shared_ptr<Type> StandardLibrary::makeConsole()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> consoleFields;

    // log function
    bool logIsVarArgs = true;
    std::vector<std::shared_ptr<Type>> logParams{};
    auto logReturnType = std::make_shared<Type>(TypeKind::Void);
    auto logFunction = std::make_shared<TypeFunction>(logIsVarArgs, logReturnType, logParams);
    consoleFields["log"] = logFunction;

    return std::make_shared<TypeObject>(consoleFields, "Console");
}

std::unordered_map<std::string, std::shared_ptr<Type>> StandardLibrary::GetBuiltins()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> builtins;
    builtins["console"] = makeConsole();
    return builtins;
}
