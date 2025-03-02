#pragma once

#pragma once
#include <string>

#include "../../../utils/FormatStream.h"
#include "../../errors/logic_errors.h"
#include "abstract_token.h"

enum class IndentType { SPACE, TAB };

struct IndentToken : public AbstractToken {
public:
    static constexpr char kSpaceChar = ' ';
    static constexpr char kTabChar = '\t';

public:
    IndentToken(IndentType type) : type_(type) {
    }

public:
    TokenType GetType() const override {
        return TokenType::INDENT;
    }

    std::string ToString() const override {
        return FormatStream() << "Indent {" << TypeToName(type_) << "}";
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
