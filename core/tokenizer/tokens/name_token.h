#pragma once

#include "../../../utils/format_stream.h"
#include "../token_types.h"
#include "abstract_token.h"

struct NameToken : public AbstractToken {
public:
    NameToken(const std::string& name) : name_(name) {
    }

    TokenType GetType() const override {
        return TokenType::NAME;
    }

    std::string GetName() const {
        return name_;
    }

    std::string ToString() const override {
        return FormatStream() << "Name {" << name_ << "}";
    }

private:
    std::string name_;
};