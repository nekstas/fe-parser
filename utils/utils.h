#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace utils {

bool Contains(std::string_view str, char c);

std::string Join(const std::vector<std::string>& strings, const std::string& delimiter);

std::string ReadFile(const std::string& filename);

}  // namespace utils
