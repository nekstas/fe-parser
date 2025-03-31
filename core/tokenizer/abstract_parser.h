#pragma once

#include "../../utils/useful_streams.h"
#include "abstract_token.h"

class AbstractParser {
public:
    virtual ~AbstractParser() {
    }

    virtual Token TryParse(CodeStream &stream) const = 0;
};

using TokenizerParser = std::unique_ptr<AbstractParser>;
