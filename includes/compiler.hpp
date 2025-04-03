#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include "utils.hpp"

#define SO_NAME "libhtml"

class Compiler
{
public:
    Compiler(const char *, const char *);

    void compile();

private:
    const char *m_filePath;
    const char *m_outputDir;
    std::vector<std::string> m_files;
    utils::OS_TYPE m_osType;

    std::string toCppStringLiteral();
    std::string generateCppSource();
};
