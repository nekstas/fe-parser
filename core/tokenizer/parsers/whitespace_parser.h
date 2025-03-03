#pragma once

#include "../../../utils/useful_streams.h"
#include "abstract_parser.h"

class WhitespaceParser : public AbstractParser {
public:
    virtual Token TryParse(CodeStream& stream) const override;
};
