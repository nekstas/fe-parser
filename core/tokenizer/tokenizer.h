#pragma once

#include <memory>
#include <vector>

#include "../../utils/istream.h"
#include "../common/code/code_stream.h"
#include "parsers/abstract_parser.h"
#include "tokens/abstract_token.h"

using Tokens = std::vector<Token>;

class Tokenizer {
public:
    Tokenizer(const CodeStream& input);

    Tokens Tokenize();

    bool IsEnd();

private:
    friend class TokenizerCreator;

    template <typename Parser, typename... Args>
    Parser* AddParser(Args&&... args) {
        auto parser = std::make_unique<Parser>(std::forward<Args>(args)...);
        auto parser_ptr = parser.get();
        parsers_.push_back(std::move(parser));
        return parser_ptr;
    }

private:
    CodeStream input_;
    std::vector<TokenizerParser> parsers_;
};
