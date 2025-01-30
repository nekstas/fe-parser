#pragma once
#include "../token_types.h"
#include "abstract_token.h"

enum class OperatorType {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER
};

struct OperatorToken : public AbstractToken {
public:
    TokenType GetType() const override {
        return TokenType::OPERATOR;
    }
};