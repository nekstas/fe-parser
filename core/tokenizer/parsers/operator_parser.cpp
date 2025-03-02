#include "operator_parser.h"

OperatorParser::OperatorParser(const SearchSet& operators) : operators_(operators) {
}

Token OperatorParser::TryParse(CodeStream& stream) const {
    // TODO: rewrite it more effectively
    std::string code;
    while (operators_.HasPrefix(code) && !operators_.Contains(code)) {
        code.push_back(stream.Get());
    }
    if (!operators_.Contains(code)) {
        return Token{};
    }
    return MakeToken<OperatorToken>(code);
}
