#include "tokenizer.h"

Tokenizer::Tokenizer(std::istream& input) : input_(input) {
}

Tokens Tokenizer::Tokenize() {
    return Tokens();
}
