#include "utils.h"

bool Contains(std::string_view str, char c) {
    return str.find(c) != std::string_view::npos;
}
