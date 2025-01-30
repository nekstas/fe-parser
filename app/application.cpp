#include "application.h"

#include <iostream>

#include "../core/tokenizer/tokenizer.h"

class Parser;

class Formatter;

int32_t Application::Run() {
    Tokenizer tokenizer(std::cin);
    Tokens tokens = tokenizer.Tokenize();
    return 0;
}
