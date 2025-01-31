#include "tokenizer.h"

Tokenizer::Tokenizer(std::istream& input) : input_(input) {
}

Tokens Tokenizer::Tokenize() {
    for (const auto& parser : parsers_) {
        if (!parser->IsActive(context_)) {
            continue;
        }

        std::optional<Token> token = parser->TryParse(input_);
        if (!token.has_value()) {
            continue;
        }

        std::cerr << "Token: " << token.value() << "\n";
        parser->ChangeContext(context_);
    }

    return Tokens();
}

bool Tokenizer::IsEnd() {
    return input_.Eof();
}
