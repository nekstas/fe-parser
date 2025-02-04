#pragma once
#include "../../../utils/FormatStream.h"
#include "../../errors/logic_errors.h"
#include "../token_types.h"
#include "abstract_token.h"

enum class OperatorType { PLUS, MINUS, MULTIPLY, DIVIDE, POWER };

struct OperatorToken : public AbstractToken {
public:
    using SubType = OperatorType;

public:
    OperatorToken(OperatorType type) : type_(type) {
    }

    TokenType GetType() const override {
        return TokenType::OPERATOR;
    }

    std::string ToString() const override {
        return FormatStream() << "Operator {" << TypeToString(type_) << "}";
    }

public:
    static constexpr std::string TypeToString(OperatorType type) {
        switch (type) {
            case OperatorType::PLUS:
                return "+";
            case OperatorType::MINUS:
                return "-";
            case OperatorType::MULTIPLY:
                return "*";
            case OperatorType::DIVIDE:
                return "/";
            case OperatorType::POWER:
                return "^";
            default:
                throw UnknownOperatorType{
                    FormatStream() << "There is not string representation for operator type ["
                                   << static_cast<size_t>(type) << "]"};
        }
    }

private:
    OperatorType type_;
};