#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "../location/location.h"
#include "../painter/painter.h"

enum ErrorKind
{
    UnterminatedString = 1001,
    UnboundName = 1002,
    UnboundProperty = 1003,
    ArgumentsCountNoMatch = 1004,
    ArgumentsTypesNoMatch = 1005,
};

class DiagnosticError
{
  private:
    ErrorKind m_Kind;
    Location m_Location;
    std::string m_Message;

  public:
    DiagnosticError(ErrorKind kind, Location location, std::string message)
        : m_Kind(kind), m_Location(location), m_Message(message) {};

    const ErrorKind &GetKind() const { return m_Kind; }
    const Location &GetLocation() const { return m_Location; }
    const std::string &GetMessage() const { return m_Message; }
};

class Diagnostics
{
  private:
    std::vector<DiagnosticError> m_Diagnostics;

  public:
    Diagnostics() : m_Diagnostics() {};

    void RegisterError(DiagnosticError error) { m_Diagnostics.push_back(error); }
    void EmitAll(std::ostream &os, std::string &fileName, std::string &fileContents);
};