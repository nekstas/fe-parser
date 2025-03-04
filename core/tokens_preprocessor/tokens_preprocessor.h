#pragma once

#include <vector>

#include "../../utils/container_stream.hpp"
#include "../../utils/format_stream.h"
#include "../../utils/useful_streams.h"
#include "../tokenizer/tokens/abstract_token.h"
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

private:
    TokensStream tokens_;
    std::vector<LineIndent> indents_;
    Tokens result_;
};
