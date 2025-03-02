#include "application.h"

#include <iostream>
#include <sstream>

#include "../core/tokenizer/tokenizer.h"
#include "../core/tokenizer/tokenizer_creator.h"

class Parser;

class Formatter;

Application::Application(size_t argc, char **argv) {
}

int32_t Application::Run() {
    code_ = std::string{"  \t2 + 2 * 2\t\n1 := 2 3 ^^^+ 4"};
    Tokenizer tokenizer = TokenizerCreator().Create(code_);
    Tokens tokens = tokenizer.Tokenize();
    return 0;
}
