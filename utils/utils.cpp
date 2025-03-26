#include "utils.h"

#include <fstream>
#include <sstream>

#include "format_stream.h"

bool utils::Contains(std::string_view str, char c) {
    return str.find(c) != std::string_view::npos;
}

std::string utils::Join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }
    std::string result = strings.front();
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    return result;
}

std::string utils::ReadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error{FormatStream() << "Can't open file \"" << filename << "\"."};
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void utils::WriteFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error{FormatStream() << "Can't open file \"" << filename << "\"."};
    }
    file << content;
}