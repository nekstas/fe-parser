#pragma once

#include "../../../utils/format_stream.h"
#include "abstract_token.h"

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
