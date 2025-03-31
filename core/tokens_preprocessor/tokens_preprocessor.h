#pragma once

#include <vector>

#include "../../utils/container_stream.hpp"
#include "../../utils/format_stream.h"
#include "../../utils/useful_streams.h"
#include "../tokenizer/abstract_token.h"
#include "line_indent.h"

using Tokens = std::vector<Token>;

class TokensPreprocessor {
public:
    TokensPreprocessor(const TokensStream& tokens);

    Tokens Process();

private:
    LineIndent CalculateLineIndent();

    void ProcessBrackets(const LineIndent& indent);

    void ProcessLineWithoutIndent();

    void AddCloseBracket();

private:
    TokensStream tokens_;
    std::vector<LineIndent> indents_;
    Tokens result_;
};
