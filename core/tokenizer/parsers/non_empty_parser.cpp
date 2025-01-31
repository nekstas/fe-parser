#include "non_empty_parser.h"

bool NonEmptyParser::IsActive(const TokenizerContext& context) const {
    return true;
}

void NonEmptyParser::ChangeContext(TokenizerContext& context) const {
    context.is_indent = false;
}
