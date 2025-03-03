#pragma once
#include <string>

#include "../../../utils/format_stream.h"
#include "abstract_token.h"

struct IntegerToken : public AbstractToken {
public:
    IntegerToken(const std::string& number) : number_(number) {
    }

public:
    TokenType GetType() const override {
        return TokenType::INTEGER;
    }

    std::string ToString() const override {
        return FormatStream() << "Integer {" << number_ << "}";
    }

private:
    std::string number_;
};