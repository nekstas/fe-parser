#include "empty_parser.h"

bool EmptyParser::IsActive(const TokenizerContext& context) const {
    return context.is_indent;
}

void EmptyParser::ChangeContext(TokenizerContext& context) const {
}
