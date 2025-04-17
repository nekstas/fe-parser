#pragma once

#include "../../../core/tokenizer/tokenizer.h"

namespace fe {

class TokenizerFactory {
public:
    TokenizerFactory() = default;

    lex::Tokenizer Create(const std::string& code) const;

private:
    void AddOperatorParser(lex::Tokenizer& tokenizer) const;

    void AddNameKeywordParser(lex::Tokenizer& tokenizer) const;
};

};  // namespace fe
