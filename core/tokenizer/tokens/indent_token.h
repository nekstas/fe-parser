#pragma once
#include "../../../utils/format_stream.h"
#include "abstract_token.h"

class UnknownIndentType : public std::logic_error {
    using std::logic_error::logic_error;
};

enum class IndentType { SPACE, TAB };

struct IndentToken : public AbstractToken {
public:
    static constexpr char kSpaceChar = ' ';
    static constexpr char kTabChar = '\t';

public:
    IndentToken(IndentType type) : type_(type) {
    }

    std::string ToString() const override {
        return FormatStream() << "INDENT {" << TypeToName(type_) << "}";
    }

    IndentType GetIndentType() const {
        return type_;
    }

public:
    static constexpr char TypeToChar(IndentType type) {
        switch (type) {
            case IndentType::SPACE:
                return kSpaceChar;
            case IndentType::TAB:
                return kTabChar;
            default:
                throw UnknownIndentType{FormatStream() << "There is not a char for indent type ["
                                                       << static_cast<size_t>(type) << "]"};
        }
    }

    static constexpr std::string TypeToName(IndentType type) {
        switch (type) {
            case IndentType::SPACE:
                return "Space";
            case IndentType::TAB:
                return "Tab";
            default:
                throw UnknownIndentType{FormatStream()
                                        << "There is not string representation for indent type ["
                                        << static_cast<size_t>(type) << "]"};
        }
    }

private:
    IndentType type_;
};
