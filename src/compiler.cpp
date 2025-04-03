#include "../includes/compiler.hpp"
Compiler::Compiler(const char *filePath, const char *outputDir) : m_filePath(filePath), m_outputDir(outputDir)
{
    std::string pathStr(filePath);
    if (pathStr.size() >= 5 && pathStr.compare(pathStr.size() - 5, 5, ".html") == 0)
    {
        m_files.push_back(filePath);
    }
    else
    {
        for (const auto &entry : std::filesystem::directory_iterator(filePath))
        {
            if (entry.path().extension() == ".html")
            {
                m_files.push_back(entry.path().string());
            }
        }
    }

    this->m_osType = utils::getOsType();

    if (m_outputDir != nullptr)
    {
        try
        {
            std::filesystem::create_directories(m_outputDir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error creating output directory: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
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
    // Iterate over all HTML files
    for (const auto &htmlFile : m_files)
    {
        // Process the HTML file
        std::string finalContent = utils::inlineAssets(htmlFile.c_str());
        std::ostringstream literalStream;
        literalStream << "R\"(" << finalContent << ")\"";
        std::string literal = literalStream.str();

        // Generate the C++ source code
        std::ostringstream cppSource;
        cppSource << "#include <string>\n\n";
        cppSource << "extern \"C\" {\n";
        cppSource << "  const char* get_html() {\n";
        cppSource << "      return " << literal << ";\n";
        cppSource << "  }\n";
        cppSource << "}\n";

        // Extract the base name of the HTML file
        std::filesystem::path pathObj(htmlFile);
        std::string baseName = pathObj.stem().string();

        // If the output directory is specified, use it
        std::string cppFilePath = (m_outputDir != nullptr ? std::string(m_outputDir) + "/" : "") + "generated_html_" + baseName + ".cpp";

        std::ofstream outfile(cppFilePath);
        if (!outfile)
        {
            std::cerr << "Erro ao criar o arquivo: " << cppFilePath << std::endl;
            exit(EXIT_FAILURE);
        }
        outfile << cppSource.str();
        outfile.close();

        // Define the output shared library name
        std::string libNameBase = (m_outputDir != nullptr ? std::string(m_outputDir) + "/" : "") + std::string(SO_NAME) + "_" + baseName;
        std::string soName = (m_osType == utils::OS_TYPE::WINDOWS)
                                 ? libNameBase + ".dll"
                                 : libNameBase + ".so";

        // Remove the existing shared library if it exists
        std::filesystem::remove(soName);

        // Mount the shared library name
        std::string compileCommand;
        if (m_osType == utils::OS_TYPE::LINUX)
        {
            compileCommand = "g++ -shared -fPIC " + cppFilePath + " -o " + soName;
        }
        else if (m_osType == utils::OS_TYPE::WINDOWS)
        {
            compileCommand = "g++ -shared " + cppFilePath + " -o " + soName;
        }
        else
        {
            std::cerr << "Sistema operacional não suportado." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Execute the compilation command
        int result = std::system(compileCommand.c_str());

        // Remove the generated C++ file
        std::filesystem::remove(cppFilePath);

        if (result == 0)
        {
            std::cout << "Library compiled successfully: " << soName << std::endl;
        }
        else
        {
            std::cerr << "Error compiling library: " << soName << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}