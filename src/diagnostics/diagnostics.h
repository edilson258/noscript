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
    UnexpectedToken = 1006,
    UnexpectedArgsSeparator = 1007,
    ExpectedFieldName = 1008,
    UnexpectedLeftSideExpression = 1009,
    ExpectedVariableName = 1010,
};

class DiagnosticError
{
  private:
    const ErrorKind m_Kind;
    const Location &m_Location;
    const std::string m_Message;

  public:
    DiagnosticError(const ErrorKind kind, const Location &location, const std::string message)
        : m_Kind(kind), m_Location(location), m_Message(message) {};

    const ErrorKind &GetKind() const { return m_Kind; }
    const Location &GetLocation() const { return m_Location; }
    const std::string &GetMessage() const { return m_Message; }
};

class Diagnostics
{
  private:
    std::vector<DiagnosticError> m_Diagnostics;

    void displayError(std::ostream &, const std::string &, const std::string &, DiagnosticError);

  public:
    Diagnostics() : m_Diagnostics() {};

    void RegisterError(DiagnosticError error) { m_Diagnostics.push_back(error); }
    void EmitAll(std::ostream &os, const std::string &fileName, const std::string &fileContents);
    void EmitNow(std::ostream &os, const std::string &fileName, const std::string &fileContents, DiagnosticError);
};
