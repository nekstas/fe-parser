#pragma once

#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

struct CloseBracketToken : public AbstractToken {
public:
    CloseBracketToken() {
    }

    bool operator==(const CloseBracketToken& other) {
        return true;
    }

public:
    std::string ToString() const override {
        return FormatStream() << "CLOSE_BRACKET";
    }
};
