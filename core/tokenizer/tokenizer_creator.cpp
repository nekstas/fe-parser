#include "tokenizer_creator.h"

#include "parsers/integer_parser.h"
#include "parsers/operator_parser.h"
#include "parsers/sequence_parsers.h"

Tokenizer TokenizerCreator::Create(const std::string& code) const {
    Tokenizer tokenizer{CodeStream(code)};
    tokenizer.AddParser<IntegerParser>();
    AddIndentParsers(tokenizer);
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
    operators.Add(":=");
    tokenizer.AddParser<OperatorParser>(operators);
}

void TokenizerCreator::AddIndentParsers(Tokenizer& tokenizer) const {
    tokenizer.AddParser<SpaceParser<IndentType::SPACE>>();
    tokenizer.AddParser<SpaceParser<IndentType::TAB>>();
}
