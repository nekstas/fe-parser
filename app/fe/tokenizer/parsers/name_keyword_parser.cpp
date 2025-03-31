#include "name_keyword_parser.h"

#include "../tokens/keyword_token.h"
#include "../tokens/name_token.h"

NameKeywordParser::NameKeywordParser(const SearchSet& keywords) : keywords_(keywords) {
}

Token NameKeywordParser::TryParse(CodeStream& stream) const {
    std::string name;
    if (!IsStartSymbol(stream.Peek())) {
        return Token{};
    }

    while (IsMiddleSymbol(stream.Peek())) {
        name.push_back(stream.Get());
    }

    if (keywords_.Contains(name)) {
        return MakeToken<KeywordToken>(name);
    }
    return MakeToken<NameToken>(name);
}

bool NameKeywordParser::IsStartSymbol(char c) const {
    return std::isalpha(c) || c == kUnderscoreChar;
}

bool NameKeywordParser::IsMiddleSymbol(char c) const {
    return IsStartSymbol(c) || std::isdigit(c);
}