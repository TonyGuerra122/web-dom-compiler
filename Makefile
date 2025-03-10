# Variáveis de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = 

# Lista todos os arquivos .cpp do diretório
SOURCES = $(wildcard src/*.cpp)
# Converte 'src/arquivo.cpp' em 'bin/arquivo.o'
OBJECTS = $(SOURCES:src/%.cpp=bin/%.o)
TARGET = bin/Program

# Regra padrão: gera o executável
all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^

# Regra para compilar os arquivos .cpp em .o e colocá-los na pasta bin
bin/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
