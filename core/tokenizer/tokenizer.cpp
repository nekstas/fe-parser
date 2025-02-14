#include "tokenizer.h"

Tokenizer::Tokenizer(const CodeStream& input) : input_(input) {
}

Tokens Tokenizer::Tokenize() {
    Tokens tokens;

    while (!IsEnd()) {
        std::optional<Token> token = std::nullopt;

        for (const auto& parser : parsers_) {
            token = parser->TryParse(input_);
            if (token.has_value()) {
                break;
            }
        }

        if (!token.has_value()) {
            std::cerr << "No token.\n";
            // TODO: throw something
            break;
        }

        if (!token.value()) {
            continue;
        }

        std::cerr << "Token: " << token.value() << "\n";
        tokens.push_back(token.value());
    }

    return tokens;
}

bool Tokenizer::IsEnd() {
    return input_.Eof();
}
