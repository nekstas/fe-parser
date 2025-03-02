#include "tokenizer.h"

Tokenizer::Tokenizer(const CodeStream& input) : input_(input) {
}

Tokens Tokenizer::Tokenize() {
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

        std::cerr << "Token: " << token->ToString() << "\n";
        tokens.push_back(token);
    }

    return tokens;
}

bool Tokenizer::IsEnd() {
    return input_.Eof();
}
