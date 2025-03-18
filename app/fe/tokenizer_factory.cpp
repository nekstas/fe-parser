#include "tokenizer_factory.h"

#include "../../core/tokenizer/parsers/integer_parser.h"
#include "../../core/tokenizer/parsers/name_parser.h"
#include "../../core/tokenizer/parsers/operator_parser.h"
#include "../../core/tokenizer/parsers/whitespace_parser.h"

Tokenizer fe::TokenizerFactory::Create(const std::string& code) const {
    Tokenizer tokenizer{CodeStream(code)};
    tokenizer.AddParser<WhitespaceParser>();
    tokenizer.AddParser<IntegerParser>();
    tokenizer.AddParser<NameParser>();
    AddOperatorParser(tokenizer);
    return tokenizer;
}

void fe::TokenizerFactory::AddOperatorParser(Tokenizer& tokenizer) const {
    SearchSet operators;
    operators.Add("+");
    operators.Add("-");
    operators.Add("*");
    operators.Add("/");
    operators.Add("^");
    operators.Add("(");
    operators.Add(")");
    operators.Add(",");
    operators.Add(".");
    operators.Add(":=");
    tokenizer.AddParser<OperatorParser>(operators);
}
