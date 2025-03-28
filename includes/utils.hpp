#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>

namespace utils
{
    enum OS_TYPE {
        WINDOWS,
        LINUX
    };

    std::string readFile(const char *);
    std::string inlineAssets(const char *);
    OS_TYPE getOsType();
}
