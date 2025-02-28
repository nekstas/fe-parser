#include "tokenizer.h"

Tokenizer::Tokenizer(const CodeStream& input) : input_(input) {
}

Tokens Tokenizer::Tokenize() {
    Tokens tokens;

    while (!IsEnd()) {
        Token token{};

        for (const auto& parser : parsers_) {
            token = parser->TryParse(input_);
            if (token) {
                break;
            }
        }

        if (!token) {
            std::cerr << "No token.\n";
            // TODO: throw something
            break;
        }

        std::cerr << "Token: " << token->ToString() << "\n";
        tokens.push_back(token);
    }

    return tokens;
}

bool Tokenizer::IsEnd() {
    return input_.Eof();
}
