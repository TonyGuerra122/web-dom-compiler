#include "../includes/utils.hpp"

namespace fs = std::filesystem;

std::string utils::readFile(const char *filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Erro ao ler o arquivo!" << std::endl;
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

    while (std::regex_search(htmlContent, match, linkRegex))
    {
        fs::path cssPath = filePathFolder / match[1].str();
        if (fs::exists(cssPath))
        {
            std::string cssContent = readFile(cssPath.c_str());
            std::string styleTag = "<style>\n" + cssContent + "\n</style>";
            htmlContent.replace(match.position(0), match.length(0), styleTag);
        }
        else
        {
            std::cerr << "Aviso: Arquivo CSS não encontrado: " << cssPath << std::endl;
        }
    }

    while (std::regex_search(htmlContent, match, scriptRegex))
    {
        std::string jsPath = filePathFolder.append(match[1].str());
        if (fs::exists(jsPath))
        {
            std::string jsContent = readFile(jsPath.c_str());
            std::string scriptTag = "<script>\n" + jsContent + "\n</script>";
            htmlContent.replace(match.position(0), match.length(0), scriptTag);
        }
        else
        {
            std::cerr << "Aviso: Arquivo JS não encontrado: " << jsPath << std::endl;
        }
    }

    return htmlContent;
}