#pragma once

#include "../../../utils/container_stream.hpp"
#include "../../../utils/useful_streams.h"
#include "abstract_parser.h"

class IntegerParser : public AbstractParser {
    // TODO: Add collapsing of (+ or -) and number later.
public:
    virtual Token TryParse(CodeStream &stream) const override;

private:
    bool IsNumberSymbol(char c) const;
};
