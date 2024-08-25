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

std::shared_ptr<Type> StandardLibrary::makeMath()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> mathFields;

    // pow function
    bool powIsVarArgs = false;
    std::vector<std::shared_ptr<Type>> powParams{std::make_shared<Type>(TypeKind::Number),
                                                 std::make_shared<Type>(TypeKind::Number)};
    auto powReturnType = std::make_shared<Type>(TypeKind::Number);
    auto powFunction = std::make_shared<TypeFunction>(powIsVarArgs, powReturnType, powParams);
    mathFields["pow"] = powFunction;

    return std::make_shared<TypeObject>(mathFields, "Math");
}

std::unordered_map<std::string, std::shared_ptr<Type>> StandardLibrary::GetBuiltins()
{
    std::unordered_map<std::string, std::shared_ptr<Type>> builtins;
    builtins["console"] = makeConsole();
    builtins["Math"] = makeMath();
    return builtins;
}
