#include "application.h"

#include <iostream>
#include <sstream>

#include "../core/tokenizer/tokenizer.h"
#include "../core/tokenizer/tokenizer_factory.h"

class Parser;

class Formatter;

Application::Application(size_t argc, char **argv) {
}

int32_t Application::Run() {
    std::stringstream input_code("2+2-3*4/5^6+-22");
    Tokenizer tokenizer = TokenizerCreator().Create(input_code);
    Tokens tokens = tokenizer.Tokenize();
    return 0;
}
