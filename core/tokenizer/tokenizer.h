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

    template <typename Parser>
    void AddParser() {
        parsers_.push_back(std::make_unique<Parser>());
    }

    Tokens Tokenize();

    bool IsEnd();

private:
    CodeStream input_;
    std::vector<TokenizerParser> parsers_;
};
