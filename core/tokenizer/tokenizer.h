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
    void AddParser(Args&&... args) {
        parsers_.push_back(std::make_unique<Parser>(std::forward<Args>(args)...));
    }

private:
    CodeStream input_;
    std::vector<TokenizerParser> parsers_;
};
