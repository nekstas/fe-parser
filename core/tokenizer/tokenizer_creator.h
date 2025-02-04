#pragma once

#include "tokenizer.h"
class TokenizerCreator {
public:
    TokenizerCreator() = default;

    Tokenizer Create(std::istream& input) const;

private:
    void AddOperatorParsers(Tokenizer& tokenizer) const;

    void AddIndentParsers(Tokenizer& tokenizer) const;
};
