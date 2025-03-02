#include "tokenizer_creator.h"

#include "parsers/integer_parser.h"
#include "parsers/name_parser.h"
#include "parsers/operator_parser.h"
#include "parsers/whitespace_parser.h"

Tokenizer TokenizerCreator::Create(const std::string& code) const {
    Tokenizer tokenizer{CodeStream(code)};
    tokenizer.AddParser<WhitespaceParser>();
    tokenizer.AddParser<IntegerParser>();
    tokenizer.AddParser<NameParser>();
    AddOperatorParser(tokenizer);
    return tokenizer;
}

void TokenizerCreator::AddOperatorParser(Tokenizer& tokenizer) const {
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
