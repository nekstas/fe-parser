#pragma once
#include "../../../utils/format_stream.h"
#include "abstract_token.h"

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

private:
    std::string number_;
};