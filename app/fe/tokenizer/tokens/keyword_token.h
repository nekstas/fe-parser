#pragma once

#include "../../../../core/tokenizer/abstract_token.h"
#include "../../../../utils/format_stream.h"

struct KeywordToken : public AbstractToken {
public:
    KeywordToken(const std::string& name) : name_(name) {
    }

    std::string GetName() const {
        return name_;
    }

    std::string ToString() const override {
        return FormatStream() << "NAME {" << name_ << "}";
    }

    bool operator==(const KeywordToken& other) {
        return name_ == other.name_;
    }

private:
    std::string name_;
};