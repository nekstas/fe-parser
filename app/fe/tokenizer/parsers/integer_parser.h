#pragma once

#include "../../../../core/tokenizer/abstract_parser.h"
#include "../../../../utils/container_stream.hpp"
#include "../../../../utils/useful_streams.h"

class IntegerParser : public AbstractParser {
public:
    virtual Token TryParse(CodeStream &stream) const override;

private:
    bool IsNumberSymbol(char c) const;
};
