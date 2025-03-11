#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include "utils.hpp"

#define SO_NAME "libhtml.so"

class Compiler
{
public:
    Compiler(const char *);

    void compile();

private:
    const char *m_filePath;

    std::string toCppStringLiteral();
    std::string generateCppSource();
};
