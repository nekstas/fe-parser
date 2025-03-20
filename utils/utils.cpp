#include "utils.h"

#include <fstream>
#include <sstream>

bool Contains(std::string_view str, char c) {
    return str.find(c) != std::string_view::npos;
}

std::string ReadFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
