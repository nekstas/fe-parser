#pragma once

#pragma once
#include <string>

#include "../../../utils/format_stream.h"
#include "abstract_token.h"

struct NewLineToken : public AbstractToken {
public:
    static constexpr char kNewLineChar = '\n';

public:
    NewLineToken() {
    }

public:
    TokenType GetType() const override {
        return TokenType::NEW_LINE;
    }

    std::string ToString() const override {
        return FormatStream() << "New line {}";
    }
};
