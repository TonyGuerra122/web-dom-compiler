#include "../includes/compiler.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Use: " << argv[0] << " <file>\n";
        return 1;
    }

    const char *filePath = argv[1];

    Compiler compiler(filePath);

    compiler.compile();

    return 0;
}
