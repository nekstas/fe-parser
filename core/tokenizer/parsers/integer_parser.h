#pragma once

#include "../../../utils/utils.h"
#include "abstract_parser.h"

class IntegerParser : public AbstractParser {
    // TODO: Add collapsing of (+ or -) and number later.
public:
    virtual std::optional<Token> TryParse(IStream &stream) const override;

private:
    bool IsNumberSymbol(int32_t c) const;
};
