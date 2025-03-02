#pragma once

#include "tokenizer.h"

class TokenizerCreator {
public:
    TokenizerCreator() = default;

    Tokenizer Create(const std::string& code) const;

private:
    void AddOperatorParser(Tokenizer& tokenizer) const;
};
