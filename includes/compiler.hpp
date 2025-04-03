#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include "utils.hpp"

#define SO_NAME "libhtml"

class Compiler
{
public:
    Compiler(const char *);

    void compile();

private:
    const char *m_filePath;
    utils::OS_TYPE m_osType;

    std::string toCppStringLiteral();
    std::string generateCppSource();
};
