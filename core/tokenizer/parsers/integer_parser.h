#pragma once

#include "../../common/code/code_stream.h"
#include "abstract_parser.h"

class IntegerParser : public AbstractParser {
    // TODO: Add collapsing of (+ or -) and number later.
public:
    virtual Token TryParse(CodeStream &stream) const override;

private:
    bool IsNumberSymbol(char c) const;
};
