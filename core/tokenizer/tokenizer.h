#pragma once

#include <memory>
#include <vector>

#include "../../utils/format_stream.h"
#include "../../utils/istream.h"
#include "../../utils/useful_streams.h"
#include "parsers/abstract_parser.h"
#include "tokens/abstract_token.h"

class TokenizerUnknownSequence : public std::runtime_error {
public:
    TokenizerUnknownSequence(size_t pos, char ch)
        : std::runtime_error{
              FormatStream() << "Unknown char or sequence of chars. Pos: " << pos
                             << ", char: " << ch << "\n"
          } {
    }
};

using Tokens = std::vector<Token>;

class Tokenizer {
public:
    Tokenizer(const CodeStream& input);

    Tokens Tokenize();

    bool IsEnd();

    template <typename Parser, typename... Args>
    void AddParser(Args&&... args) {
        parsers_.push_back(std::make_unique<Parser>(std::forward<Args>(args)...));
    }

private:
    CodeStream input_;
    std::vector<TokenizerParser> parsers_;
};
