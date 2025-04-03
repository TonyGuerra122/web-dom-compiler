# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -fsanitize=address
LDFLAGS = -fsanitize=address

# List all .cpp files in the directory
SOURCES = $(wildcard src/*.cpp)
# Converts 'src/file.cpp' to 'bin/file.o'
OBJECTS = $(SOURCES:src/%.cpp=bin/%.o)
TARGET = bin/Program

# Default rule: generate the executable
all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^

# Rule to compile .cpp files into .o and place them in the bin folder
bin/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
