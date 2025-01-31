#pragma once

#include "../../../utils/utils.h"
#include "non_empty_parser.h"

class IntegerParser : public NonEmptyParser {
    static constexpr std::string_view kSignSymbols = "+-";

public:
    virtual std::optional<Token> TryParse(IStream& stream) const override;

private:
    bool IsSignSymbol(char c) const;

    bool IsNumberStart(IStream& in) const;

    bool IsNumberSymbol(int c) const;
};
