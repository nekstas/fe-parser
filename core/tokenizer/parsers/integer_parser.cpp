#include "integer_parser.h"

#include "../tokens/integer_token.h"

Token IntegerParser::TryParse(CodeStream& stream) const {
    std::string number_str;
    while (IsNumberSymbol(stream.Peek())) {
        number_str.push_back(stream.Get());
    }

    if (number_str.empty()) {
        return Token{};
    }
    return MakeToken<IntegerToken>(number_str);
}

bool IntegerParser::IsNumberSymbol(char c) const {
    return std::isdigit(c);
}
