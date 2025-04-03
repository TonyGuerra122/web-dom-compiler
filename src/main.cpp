#include "../includes/compiler.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cerr << "Use: " << argv[0] << " <file> [output_dir]\n";
        return 1;
    }

    const char *filePath = argv[1];
    const char *outputDir = argv[2];

    Compiler compiler(filePath, outputDir);

    compiler.compile();

    return 0;
}
