#pragma once
#include "../tokens/operator_token.h"
#include "abstract_parser.h"

template <typename TokenT, TokenT::SubType type>
class SequenceParser : public AbstractParser {
private:
    static constexpr std::string kSequenceString = TokenT::TypeToString(type);

public:
    virtual Token TryParse(CodeStream& stream) const override {
        if (!CheckMatch(stream, kSequenceString)) {
            return Token{};
        }

        stream.Skip(kSequenceString.size());
        return MakeToken<TokenT>(type);
    }

private:
    bool CheckMatch(CodeStream& stream, const std::string substr) const {
        for (size_t i = 0; i < substr.size(); ++i) {
            if (stream.Peek(i) != substr[i]) {
                return false;
            }
        }
        return true;
    }
};
