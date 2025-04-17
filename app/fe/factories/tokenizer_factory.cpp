#include "tokenizer_factory.h"

#include "../tokenizer/parsers/integer_parser.h"
#include "../tokenizer/parsers/name_keyword_parser.h"
#include "../tokenizer/parsers/operator_parser.h"
#include "../tokenizer/parsers/whitespace_parser.h"

Tokenizer fe::TokenizerFactory::Create(const std::string& code) const {
    Tokenizer tokenizer{CodeStream(code)};
    tokenizer.AddParser<WhitespaceParser>();
    tokenizer.AddParser<IntegerParser>();
    AddNameKeywordParser(tokenizer);
    AddOperatorParser(tokenizer);
    return tokenizer;
}

void fe::TokenizerFactory::AddOperatorParser(Tokenizer& tokenizer) const {
    SearchSet operators({"+", "-", "*", "/", "^", "(", ")", ",", ".", ":="});
    tokenizer.AddParser<OperatorParser>(operators);
}

void fe::TokenizerFactory::AddNameKeywordParser(Tokenizer& tokenizer) const {
    SearchSet keywords({"let", "as", "where", "module", "import"});
    tokenizer.AddParser<NameKeywordParser>(keywords);
}
