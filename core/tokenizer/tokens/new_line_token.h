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
    std::string ToString() const override {
        return FormatStream() << "NEW_LINE";
    }

    bool operator==(const NewLineToken& other) {
        return true;
    }
};
