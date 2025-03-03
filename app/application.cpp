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
    code_ = std::string{
        "module MyLinalg where\n"
        "   import Unused\n"
        "\n"
        "   let solve(A, b  )   :=   LA.solve( A  ,  b  )\n"
        "\n"
        "   import numpy.linalg as LA\n"
        "\n"
        "\n"
        "let some_root(a , b  , c  ) :=   (  b +   math.sqrt(( discriminant(a, b, c ) ) )) / a "
        "where\n"
        "\n"
        "  let discriminant(   a, b,c   ) := (b ^ 2) - 4 * (a * c)\n"
        "\n"
        "\n"
        "  import math"};
    Tokenizer tokenizer = TokenizerCreator().Create(code_);
    Tokens tokens = tokenizer.Tokenize();
    return 0;
}
