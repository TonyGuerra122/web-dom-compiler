#include "../includes/compiler.hpp"

Compiler::Compiler(const char *filePath) : m_filePath(filePath)
{
}

std::string Compiler::toCppStringLiteral()
{
    const std::string content = utils::readFile(m_filePath);

    const std::string finalContent = utils::inlineAssets(m_filePath);

    std::ostringstream output;
    output << "R\"(" << finalContent << ")\"";

    return output.str();
}

std::string Compiler::generateCppSource()
{
    std::ostringstream cppSource;
    cppSource << "#include <string>\n\n";
    cppSource << "extern \"C\" {\n";
    cppSource << "  const char* get_html" << "() {\n";
    cppSource << "      return " << toCppStringLiteral() << ";\n";
    cppSource << "  }\n";
    cppSource << "}\n";

    return cppSource.str();
}

void Compiler::compile()
{
    std::string cppSource = generateCppSource();

    std::string outputCppFile = "generated_html.cpp";
    std::ofstream outfile(outputCppFile);
    if (!outfile)
    {
        std::cerr << "Error creating file: " << outputCppFile << std::endl;
        exit(EXIT_FAILURE);
    }

    outfile << cppSource;
    outfile.close();

    std::filesystem::remove(SO_NAME);

    std::string compileCommand = "g++ -shared -fPIC " + outputCppFile + " -o " + SO_NAME;
    int result = std::system(compileCommand.c_str());

    std::filesystem::remove(outputCppFile);

    if (result == 0)
    {
        std::cout << "Shared library created successfully: " << SO_NAME << std::endl;
    }
    else
    {
        std::cerr << "Error compiling library." << std::endl;
        exit(EXIT_FAILURE);
    }
}
