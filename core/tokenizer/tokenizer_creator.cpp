#include "tokenizer_creator.h"

#include "parsers/integer_parser.h"
#include "parsers/sequence_parsers.h"

Tokenizer TokenizerCreator::Create(const std::string& code) const {
    Tokenizer tokenizer{CodeStream(code)};
    tokenizer.AddParser<IntegerParser>();
    AddIndentParsers(tokenizer);
    AddOperatorParsers(tokenizer);
    return tokenizer;
}

void TokenizerCreator::AddOperatorParsers(Tokenizer& tokenizer) const {
    tokenizer.AddParser<OperatorParser<OperatorType::PLUS>>();
    tokenizer.AddParser<OperatorParser<OperatorType::MINUS>>();
    tokenizer.AddParser<OperatorParser<OperatorType::MULTIPLY>>();
    tokenizer.AddParser<OperatorParser<OperatorType::DIVIDE>>();
    tokenizer.AddParser<OperatorParser<OperatorType::POWER>>();
}

void TokenizerCreator::AddIndentParsers(Tokenizer& tokenizer) const {
    tokenizer.AddParser<SpaceParser<IndentType::SPACE>>();
    tokenizer.AddParser<SpaceParser<IndentType::TAB>>();
}
