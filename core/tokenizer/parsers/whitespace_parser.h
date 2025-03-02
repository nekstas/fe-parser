#pragma once

#include "../../common/code/code_stream.h"
#include "abstract_parser.h"

class WhitespaceParser : public AbstractParser {
public:
    virtual Token TryParse(CodeStream& stream) const override;
};
