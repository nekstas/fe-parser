#pragma once
#include "../../../utils/FormatStream.h"
#include "../token_types.h"
#include "abstract_token.h"

struct OperatorToken : public AbstractToken {
public:
    OperatorToken(const std::string& code) : code_(code) {
    }

    TokenType GetType() const override {
        return TokenType::OPERATOR;
    }

    std::string GetCode() const {
        return code_;
    }

    std::string ToString() const override {
        return FormatStream() << "Operator {" << code_ << "}";
    }

private:
    std::string code_;
};