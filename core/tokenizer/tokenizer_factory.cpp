#include "tokenizer_factory.h"

#include "parsers/integer_parser.h"
#include "parsers/operator_parser.hpp"

Tokenizer TokenizerCreator::Create(std::istream& input) const {
    Tokenizer tokenizer = Tokenizer(input);
    tokenizer.AddParser<OperatorParser<OperatorType::PLUS>>();
    tokenizer.AddParser<OperatorParser<OperatorType::MINUS>>();
    tokenizer.AddParser<OperatorParser<OperatorType::MULTIPLY>>();
    tokenizer.AddParser<OperatorParser<OperatorType::DIVIDE>>();
    tokenizer.AddParser<OperatorParser<OperatorType::POWER>>();
    tokenizer.AddParser<IntegerParser>();
    return tokenizer;
}
