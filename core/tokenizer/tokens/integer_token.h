#pragma once
#include <string>

#include "abstract_token.h"

struct IntegerToken : public AbstractToken {
public:
    std::string number;

    TokenType GetType() const override {
        return TokenType::INTEGER;
    }
};