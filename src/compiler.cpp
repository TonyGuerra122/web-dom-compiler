#include "../includes/compiler.hpp"

Compiler::Compiler(const char *filePath, DOM_TYPE domType) : m_filePath(filePath), m_domType(domType)
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
    const char *type = getTypeName();

    std::ostringstream cppSource;
    cppSource << "#include <string>\n\n";
    cppSource << "extern \"C\" {\n";
    cppSource << "  const char* get_" << type << "() {\n";
    cppSource << "      return " << toCppStringLiteral() << ";\n";
    cppSource << "  }\n";
    cppSource << "}\n";

    return cppSource.str();
}

const char *Compiler::getTypeName()
{
    switch (m_domType)
    {
    case HTML:
        return "html";
    case CSS:
        return "css";
    case JAVASCRIPT:
        return "javascript";
    default:
        return "unknown";
    }
}

void Compiler::compile()
{
    std::string cppSource = generateCppSource();

    std::string outputCppFile = "generated_" + std::string(getTypeName()) + ".cpp";
    std::ofstream outfile(outputCppFile);
    if (!outfile)
    {
        std::cerr << "Erro ao criar o arquivo: " << outputCppFile << std::endl;
        exit(EXIT_FAILURE);
    }

    outfile << cppSource;
    outfile.close();

    std::string outputSoFile = "lib" + std::string(getTypeName()) + ".so"; // Corrigido nome do .so

    std::filesystem::remove(outputSoFile);

    std::string compileCommand = "g++ -shared -fPIC " + outputCppFile + " -o " + outputSoFile;
    int result = std::system(compileCommand.c_str());

    std::filesystem::remove(outputCppFile);

    if (result == 0)
    {
        std::cout << "Biblioteca compartilhada criada com sucesso: " << outputSoFile << std::endl;
    }
    else
    {
        std::cerr << "Erro ao compilar a biblioteca." << std::endl;
        exit(EXIT_FAILURE);
    }
}

DOM_TYPE Compiler::charToDomType(const char *type)
{
    std::string strType(type);
    if (strType == "html")
    {
        return HTML;
    }
    else if (strType == "css")
    {
        return CSS;
    }
    else if (strType == "javascript")
    {
        return JAVASCRIPT;
    }
    else
    {
        std::cerr << "Erro: Tipo inválido! Use 'html', 'css' ou 'js'." << std::endl;
        exit(EXIT_FAILURE);
    }
}
