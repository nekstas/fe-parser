#pragma once

#include "../../../utils/istream.h"
#include "../tokens/abstract_token.h"

class AbstractParser {
public:
    virtual ~AbstractParser() {
    }

    virtual std::optional<Token> TryParse(CodeStream &stream) const = 0;
};

using TokenizerParser = std::unique_ptr<AbstractParser>;
