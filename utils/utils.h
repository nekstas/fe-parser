#pragma once

#include <string_view>
#include <string>

bool Contains(std::string_view str, char c);

std::string ReadFile(const std::string& filename);
