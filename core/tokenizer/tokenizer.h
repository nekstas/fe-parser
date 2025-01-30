#pragma once
#include <memory>
#include <vector>

#include "../../utils/istream.h"
#include "tokens/abstract_token.h"

using Token = std::shared_ptr<AbstractToken>;
using Tokens = std::vector<Token>;

class Tokenizer {
public:
    Tokenizer(std::istream& input);

    Tokens Tokenize();

    bool IsEnd();

private:
    IStream input_;
};