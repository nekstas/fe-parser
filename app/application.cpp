#include "application.h"

#include <iostream>

#include "../core/parser/parser.h"
#include "../core/tokenizer/tokenizer.h"
#include "../core/tokens_preprocessor/tokens_preprocessor.h"
#include "../utils/utils.h"
#include "fe/factories/parser_factory.h"
#include "fe/factories/tokenizer_factory.h"

class Formatter;

void DebugTokensList(const std::string& hint, const Tokens& tokens) {
    std::cerr << hint << " [count = " << tokens.size() << "]" << "\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cerr << "[" << i + 1 << "]: " << tokens[i] << "\n";
    }
    std::cerr << "\n";
}

Application::Application(size_t argc, char** argv) {
    // TODO: change it later
    if (argc < 2) {
        throw std::runtime_error{"Incorrect number of cmd args."};
    }
    code_ = ReadFile(argv[1]);
}

int32_t Application::Run() {
    Tokenizer tokenizer = fe::TokenizerFactory().Create(code_);
    Tokens tokens = tokenizer.Tokenize();
    DebugTokensList("Tokens from Tokenizer.", tokens);

    TokensPreprocessor preprocessor(tokens);
    Tokens new_tokens = preprocessor.Process();
    DebugTokensList("Tokens from Preprocessor.", new_tokens);

    Parser parser = fe::ParserFactory().Create(new_tokens);
    syntax_tree::NodePtr result = parser.Parse();

    std::cerr << "SyntaxTree from Parser.\n";
    std::cerr << syntax_tree::Node::GetStringRepresentation(result);

    return 0;
}
