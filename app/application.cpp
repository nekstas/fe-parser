#include "application.h"

#include <iostream>

#include "../core/tokenizer/tokenizer.h"
#include "../core/tokenizer/tokenizer_factory.h"

class Parser;

class Formatter;

Application::Application(size_t argc, char **argv) {
}

int32_t Application::Run() {
    Tokenizer tokenizer = TokenizerCreator().Create(std::cin);
    Tokens tokens = tokenizer.Tokenize();
    return 0;
}
