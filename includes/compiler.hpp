#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include "utils.hpp"

enum DOM_TYPE
{
    HTML,
    CSS,
    JAVASCRIPT
};

class Compiler
{
public:
    Compiler(const char *, DOM_TYPE);

    void compile();
    static DOM_TYPE charToDomType(const char *);

private:
    const char *m_filePath;
    DOM_TYPE m_domType;

    std::string toCppStringLiteral();
    std::string generateCppSource();

    const char *getTypeName();
};
