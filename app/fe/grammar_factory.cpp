#include "grammar_factory.h"

#include "../../core/parser/grammar/rules/named_rule.h"
#include "../../core/parser/grammar/rules/token_type_rule.hpp"
#include "../../core/tokenizer/tokens/integer_token.h"
#include "../../core/tokenizer/tokens/name_token.h"

Grammar fe::GrammarFactory::Create() const {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::TokenTypeRule;

    Grammar grammar;
    grammar.SetMainRule("program");

    grammar.AddRule("number", MakeRule<TokenTypeRule<IntegerToken>>());
    grammar.AddRule("identifier", MakeRule<TokenTypeRule<NameToken>>());

    grammar.AddRule("program", MakeRule<NamedRule>("number"));
    return grammar;
}
