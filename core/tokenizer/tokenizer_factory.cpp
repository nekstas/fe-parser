#include "tokenizer_factory.h"

#include "parsers/integer_parser.h"

Tokenizer TokenizerCreator::Create(std::istream& input) const {
    Tokenizer tokenizer = Tokenizer(input);
    tokenizer.AddParser<IntegerParser>();
    return tokenizer;
}
