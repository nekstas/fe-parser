#pragma once
#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

struct IntegerToken : public AbstractToken {
public:
    IntegerToken(const std::string& number) : number_(number) {
    }

public:
    std::string ToString() const override {
        return FormatStream() << "INTEGER {" << number_ << "}";
    }

    bool operator==(const IntegerToken& other) {
        return number_ == other.number_;
    }

    const std::string& GetValue() const {
        return number_;
    }

private:
    std::string number_;
};