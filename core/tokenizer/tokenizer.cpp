#include "tokenizer.h"

lex::Tokenizer::Tokenizer(const CodeStream& input) : input_(input) {
}

lex::Tokens lex::Tokenizer::Tokenize() {
    Tokens tokens;

    while (!IsEnd()) {
        Token token{};
        size_t saved_pos = input_.GetPos();

        for (const auto& parser : parsers_) {
            token = parser->TryParse(input_);
            if (token) {
                break;
            }
            input_.Restore(saved_pos);
        }

        if (!token) {
            throw TokenizerUnknownSequence{input_.GetPos(), input_.Peek()};
        }
        tokens.push_back(token);
    }

    return tokens;
}

bool lex::Tokenizer::IsEnd() {
    return input_.Eof();
}
