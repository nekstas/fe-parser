#pragma once
#include "../token_types.h"
#include "base_token.h"

enum class OperatorType {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER
};

struct OperatorToken : public BaseToken {
public:
    TokenType GetType() const override {
        return TokenType::OPERATOR;
    }
};