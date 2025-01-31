#include "integer_parser.h"

#include "../tokens/integer_token.h"

std::optional<Token> IntegerParser::TryParse(IStream& stream) const {
    if (!IsNumberStart(stream)) {
        return std::nullopt;
    }

    std::string number_str;
    number_str.push_back(stream.Get());
    while (IsNumberSymbol(stream.Peek())) {
        number_str.push_back(stream.Get());
    }

    return MakeToken<IntegerToken>(number_str);
}

bool IntegerParser::IsSignSymbol(char c) const {
    return Contains(kSignSymbols, c);
}

bool IntegerParser::IsNumberStart(IStream& in) const {
    return IsNumberSymbol(in.Peek()) || (IsSignSymbol(in.Peek()) && IsNumberSymbol(in.Peek(1)));
}

bool IntegerParser::IsNumberSymbol(int c) const {
    return std::isdigit(c);
}
