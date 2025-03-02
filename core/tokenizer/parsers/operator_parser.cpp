#include "operator_parser.h"

OperatorParser::OperatorParser(const SearchSet& operators) : operators_(operators) {
}

Token OperatorParser::TryParse(CodeStream& stream) const {
    // TODO: rewrite it more effectively
    std::string code;
    while (operators_.HasPrefix(code) && !operators_.Contains(code)) {
        code.push_back(stream.Peek(code.size()));
    }
    if (!operators_.Contains(code)) {
        return Token{};
    }
    // TODO: maybe just recover initial position, if token is empty
    stream.Skip(code.size());
    return MakeToken<OperatorToken>(code);
}
