#pragma once
#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

struct OperatorToken : public AbstractToken {
public:
    OperatorToken(const std::string& code) : code_(code) {
    }

    std::string GetCode() const {
        return code_;
    }

    std::string ToString() const override {
        return FormatStream() << "OPERATOR \"" << code_ << "\"";
    }

    bool operator==(const OperatorToken& other) {
        return code_ == other.code_;
    }

private:
    std::string code_;
};