#pragma once
#include <string>

#include "base_token.h"

struct IntegerToken : public BaseToken {
public:
    std::string number;

    TokenType GetType() const override {
        return TokenType::INTEGER;
    }
};