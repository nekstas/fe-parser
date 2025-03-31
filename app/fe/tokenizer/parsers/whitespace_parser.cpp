#include "whitespace_parser.h"

#include "../tokens/indent_token.h"
#include "../tokens/new_line_token.h"

Token WhitespaceParser::TryParse(CodeStream& stream) const {
    switch (stream.Get()) {
        case IndentToken::kSpaceChar:
            return MakeToken<IndentToken>(IndentType::SPACE);
        case IndentToken::kTabChar:
            return MakeToken<IndentToken>(IndentType::TAB);
        case NewLineToken::kNewLineChar:
            return MakeToken<NewLineToken>();
    }
    return Token();
}
