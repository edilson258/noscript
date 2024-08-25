#pragma once

#include <memory>
#include <unordered_map>

#include "../types/types.h"

class StandardLibrary
{
  private:
    static std::shared_ptr<Type> makeConsole();
    static std::shared_ptr<Type> makeMath();

  public:
    static std::unordered_map<std::string, std::shared_ptr<Type>> GetBuiltins();
};
