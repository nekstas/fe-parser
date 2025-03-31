#pragma once
#include <string>

#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

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
