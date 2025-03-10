#include "../includes/compiler.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0] << " <arquivo> <tipo: html/css/js>\n";
        return 1;
    }

    const char *filePath = argv[1];
    const char *type = argv[2];

    // Converte o tipo para DOM_TYPE
    DOM_TYPE domType = Compiler::charToDomType(type);

    // Criamos um objeto normal (stack) para evitar vazamento de memória
    Compiler compiler(filePath, domType);

    // Compila o arquivo para .so
    compiler.compile();

    return 0; // Retorna sucesso
}
