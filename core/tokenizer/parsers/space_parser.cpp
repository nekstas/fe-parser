#include "space_parser.h"

bool SpaceParser::IsActive(const TokenizerContext& context) const {
    return !context.is_indent;
}

std::optional<Token> SpaceParser::TryParse(IStream& stream) const {
    if (!IsSpace(stream.Peek())) {
        return std::nullopt;
    }

    stream.Ignore();
    return Token();
}

void SpaceParser::ChangeContext(TokenizerContext& context) const {
}

bool SpaceParser::IsSpace(int32_t c) const {
    return std::isspace(c);
}
