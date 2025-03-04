#pragma once

#include "../../../utils/format_stream.h"
#include "abstract_token.h"

struct CloseBracketToken : public AbstractToken {
public:
    CloseBracketToken() {
    }

public:
    std::string ToString() const override {
        return FormatStream() << "CLOSE_BRACKET";
    }
};
