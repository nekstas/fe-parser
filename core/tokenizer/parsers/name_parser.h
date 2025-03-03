#pragma once

#include "../../../utils/useful_streams.h"
#include "abstract_parser.h"

class NameParser : public AbstractParser {
private:
    static constexpr char kUnderscoreChar = '_';

public:
    virtual Token TryParse(CodeStream &stream) const override;

private:
    bool IsStartSymbol(char c) const;

    bool IsMiddleSymbol(char c) const;
};
