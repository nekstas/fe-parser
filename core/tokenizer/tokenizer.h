#pragma once
#include <memory>
#include <vector>

#include "tokens/base_token.h"

using Token = std::shared_ptr<BaseToken>;
using Tokens = std::vector<Token>;

class Tokenizer {
public:
    Tokenizer(std::istream& input);

    Tokens Tokenize();
};