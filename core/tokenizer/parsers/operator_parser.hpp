#pragma once
#include "../tokens/operator_token.h"
#include "non_empty_parser.h"

template <OperatorType type>
class OperatorParser : public NonEmptyParser {
private:
    static constexpr std::string kOperatorString = OperatorToken::TypeToString(type);

public:
    virtual std::optional<Token> TryParse(IStream& stream) const override {
        if (!CheckMatch(stream, kOperatorString)) {
            return std::nullopt;
        }
        stream.Skip(kOperatorString.size());

        return MakeToken<OperatorToken>(type);
    }

private:
    bool CheckMatch(IStream& stream, const std::string substr) const {
        for (size_t i = 0; i < substr.size(); ++i) {
            if (stream.Peek(i) != substr[i]) {
                return false;
            }
        }

        return true;
    }
};
