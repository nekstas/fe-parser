#include "parser_factory.h"

#include "grammar_factory.h"

Parser fe::ParserFactory::Create(const std::vector<Token>& tokens) const {
    Grammar lang_grammar = fe::GrammarFactory().Create();
    return Parser(lang_grammar, tokens);
}
