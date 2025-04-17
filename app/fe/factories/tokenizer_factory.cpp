#include "tokenizer_factory.h"

#include "../tokenizer/parsers/integer_parser.h"
#include "../tokenizer/parsers/name_keyword_parser.h"
#include "../tokenizer/parsers/operator_parser.h"
#include "../tokenizer/parsers/whitespace_parser.h"

lex::Tokenizer fe::TokenizerFactory::Create() const {
    lex::Tokenizer tokenizer;
    tokenizer.AddParser<WhitespaceParser>();
    tokenizer.AddParser<IntegerParser>();
    AddNameKeywordParser(tokenizer);
    AddOperatorParser(tokenizer);
    return tokenizer;
}

void fe::TokenizerFactory::AddOperatorParser(lex::Tokenizer& tokenizer) const {
    SearchSet operators({"+", "-", "*", "/", "^", "(", ")", ",", ".", ":="});
    tokenizer.AddParser<OperatorParser>(operators);
}

void fe::TokenizerFactory::AddNameKeywordParser(lex::Tokenizer& tokenizer) const {
    SearchSet keywords({"let", "as", "where", "module", "import"});
    tokenizer.AddParser<NameKeywordParser>(keywords);
}
