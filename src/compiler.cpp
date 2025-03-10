#include "../includes/compiler.hpp"

Compiler::Compiler(const char *filePath, DOM_TYPE domType) : m_file(filePath), m_domType(domType)
{
    if (!m_file)
    {
        std::cerr << "Erro ao abrir o arquivo: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
}

Compiler::~Compiler()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

std::string Compiler::readFileToString()
{
    if (!m_file.is_open())
    {
        std::cerr << "Erro ao ler o arquivo!" << std::endl;
        return "";
    }

    m_file.clear();                 // Limpa flags de erro, se houver
    m_file.seekg(0, std::ios::beg); // Retorna ao início do arquivo

    std::ostringstream buffer;
    buffer << m_file.rdbuf();
    return buffer.str();
}

std::string Compiler::toCppStringLiteral()
{
    std::string content = readFileToString();

    std::ostringstream output;
    output << "R\"(" << content << ")\"";

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

    // Remover o arquivo antigo antes de compilar
    std::string removeCommand = "rm -f " + outputSoFile;
    std::system(removeCommand.c_str());

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
