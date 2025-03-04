#include "tokens_preprocessor.h"

#include "../tokenizer/tokens/close_bracket.h"
#include "../tokenizer/tokens/indent_token.h"
#include "../tokenizer/tokens/new_line_token.h"
#include "../tokenizer/tokens/open_bracket.h"

TokensPreprocessor::TokensPreprocessor(const TokensStream& tokens) : tokens_(tokens) {
}

Tokens TokensPreprocessor::Process() {
    indents_ = {{0, 0}};

    while (!tokens_.Eof()) {
        LineIndent indent = CalculateLineIndent();

        if (IsToken<NewLineToken>(tokens_.Peek())) {
            tokens_.Ignore();
            continue;
        } else if (!indent.IsNormal()) {
            throw MixedIndentError(indent);
        }

        ProcessBrackets(indent);
        ProcessLineWithoutIndent();
    }

    while (indents_.size() > 1) {
        indents_.pop_back();
        result_.push_back(MakeToken<CloseBracketToken>());
    }

    return result_;
}

LineIndent TokensPreprocessor::CalculateLineIndent() {
    LineIndent indent;

    while (IsToken<IndentToken>(tokens_.Peek())) {
        auto token = ConvertTokenTo<IndentToken>(tokens_.Get());
        switch (token->GetIndentType()) {
            case IndentType::SPACE:
                ++indent.spaces;
                break;
            case IndentType::TAB:
                ++indent.tabs;
                break;
            default:
                throw UnknownIndentType{"Found unknown indent type in TokensPreprocessor."};
        }
    }

    return indent;
}

void TokensPreprocessor::ProcessBrackets(const LineIndent& indent) {
    while (indent.IsLess(indents_.back())) {
        indents_.pop_back();
        result_.push_back(MakeToken<CloseBracketToken>());
    }

    if (indent.IsGreater(indents_.back())) {
        if (!result_.empty() && IsToken<NewLineToken>(result_.back())) {
            result_.pop_back();
        }
        indents_.push_back(indent);
        result_.push_back(MakeToken<OpenBracketToken>());
    } else if (indent != indents_.back()) {
        throw IndentsMismatchError(indent, indents_.back());
    }
}

void TokensPreprocessor::ProcessLineWithoutIndent() {
    while (!tokens_.Eof() && !IsToken<NewLineToken>(tokens_.Peek())) {
        Token token = tokens_.Get();
        if (!IsToken<IndentToken>(token)) {
            result_.push_back(token);
        }
    }

    result_.push_back(MakeToken<NewLineToken>());
    if (!tokens_.Eof()) {
        tokens_.Ignore();
    }
}
