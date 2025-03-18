#pragma once

#include "../../core/tokenizer/tokenizer.h"

namespace fe {

class TokenizerFactory {
public:
    TokenizerFactory() = default;

    Tokenizer Create(const std::string& code) const;

private:
    void AddOperatorParser(Tokenizer& tokenizer) const;
};

};  // namespace fe
