#pragma once

#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

struct OpenBracketToken : public AbstractToken {
public:
    OpenBracketToken() {
    }

public:
    std::string ToString() const override {
        return FormatStream() << "OPEN_BRACKET";
    }

    bool operator==(const OpenBracketToken& other) {
        return true;
    }
};
