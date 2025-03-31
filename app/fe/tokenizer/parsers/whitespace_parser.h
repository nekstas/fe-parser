#pragma once

#include "../../../../core/tokenizer/abstract_parser.h"
#include "../../../../utils/useful_streams.h"

class WhitespaceParser : public AbstractParser {
public:
    virtual Token TryParse(CodeStream& stream) const override;
};
