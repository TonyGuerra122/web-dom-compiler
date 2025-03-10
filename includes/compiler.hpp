#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>

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
    ~Compiler();

    void compile();
    static DOM_TYPE charToDomType(const char *);

private:
    std::ifstream m_file;
    DOM_TYPE m_domType;

    std::string toCppStringLiteral();
    std::string generateCppSource();
    std::string readFileToString(); // ❌ REMOVIDO static!

    const char *getTypeName();
};
