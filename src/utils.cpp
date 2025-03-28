#include "../includes/utils.hpp"

namespace fs = std::filesystem;

std::string utils::readFile(const char *filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Error reading file!" << std::endl;
        return "";
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    std::ostringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

std::string utils::inlineAssets(const char *filePath)
{
    fs::path filePathRoot(filePath);
    fs::path filePathFolder = filePathRoot.parent_path();

    std::string htmlContent = readFile(filePath);

    std::regex linkRegex(R"(<link\s+[^>]*href=["']([^"']+.css)["'][^>]*>)");
    std::regex scriptRegex(R"(<script\s+[^>]*src=["']([^"']+.js)["'][^>]*></script>)");

    std::smatch match;

    // Processa arquivos CSS
    while (std::regex_search(htmlContent, match, linkRegex))
    {
        fs::path cssPath = filePathFolder / match[1].str();
        if (fs::exists(cssPath))
        {
            // Converte o caminho para std::string antes de chamar readFile
            std::string cssContent = readFile(cssPath.string().c_str());
            std::string styleTag = "<style>\n" + cssContent + "\n</style>";
            htmlContent.replace(match.position(0), match.length(0), styleTag);
        }
        else
        {
            std::cerr << "Warning: CSS file not found: " << cssPath << std::endl;
        }
    }

    // Processa arquivos JavaScript
    while (std::regex_search(htmlContent, match, scriptRegex))
    {
        // Cria um novo objeto path para o arquivo JS, sem modificar filePathFolder
        fs::path jsFilePath = filePathFolder / match[1].str();
        if (fs::exists(jsFilePath))
        {
            std::string jsContent = readFile(jsFilePath.string().c_str());
            std::string scriptTag = "<script>\n" + jsContent + "\n</script>";
            htmlContent.replace(match.position(0), match.length(0), scriptTag);
        }
        else
        {
            std::cerr << "Warning: JS file not found: " << jsFilePath << std::endl;
        }
    }

    return htmlContent;
}

utils::OS_TYPE utils::getOsType()
{
#if defined(_WIN32) || defined(_WIN64)
    return OS_TYPE::WINDOWS;
#else
    return OS_TYPE::LINUX;
#endif
}
