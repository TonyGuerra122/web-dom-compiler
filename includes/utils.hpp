#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>

namespace utils
{
    std::string readFile(const char *);
    std::string inlineAssets(const char *);
}
