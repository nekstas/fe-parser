#include "name_parser.h"

#include "../tokens/name_token.h"

Token NameParser::TryParse(CodeStream& stream) const {
    std::string name;
    if (!IsStartSymbol(stream.Peek())) {
        return Token{};
    }

    while (IsMiddleSymbol(stream.Peek())) {
        name.push_back(stream.Get());
    }
    return MakeToken<NameToken>(name);
}

bool NameParser::IsStartSymbol(char c) const {
    return std::isalpha(c) || c == kUnderscoreChar;
}

bool NameParser::IsMiddleSymbol(char c) const {
    return IsStartSymbol(c) || std::isdigit(c);
}
