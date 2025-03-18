#include "application.h"

#include <iostream>

#include "../core/parser/parser.h"
#include "../core/tokenizer/tokenizer.h"
#include "../core/tokens_preprocessor/tokens_preprocessor.h"
#include "fe/parser_factory.h"
#include "fe/tokenizer_factory.h"

class Formatter;

void DebugTokensList(const std::string& hint, const Tokens& tokens) {
    std::cerr << hint << " [count = " << tokens.size() << "]" << "\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cerr << "[" << i + 1 << "]: " << tokens[i] << "\n";
    }
    std::cerr << "\n";
}

Application::Application(size_t argc, char **argv) {
}

int32_t Application::Run() {
//    code_ = std::string{
//        "module MyLinalg where\n"
//        "   import Unused\n"
//        "\n"
//        "   let solve(A, b  )   :=   LA.solve( A  ,  b  )\n"
//        "\n"
//        "\n"
//        "   let d := e"
//        "   import numpy.linalg as LA\n"
//        "\n"
//        "\n"
//        "let some_root(a , b  , c  ) :=   (  b +   math.sqrt(( discriminant(a, b, c ) ) )) / a "
//        "where\n"
//        "\n"
//        "  let discriminant(   a, b,c   ) := (b ^ 2) - 4 * (a * c)\n"
//        "\n"
//        "\n"
//        "  import math"};

    code_ = "1 + 2 + 3 + 4 where 5";

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
