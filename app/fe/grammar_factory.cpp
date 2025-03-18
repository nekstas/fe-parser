#include "grammar_factory.h"

#include "../../core/parser/grammar/rules/named_rule.h"
#include "../../core/parser/grammar/rules/sequence_rule.h"
#include "../../core/parser/grammar/rules/token_type_rule.hpp"
#include "../../core/parser/grammar/rules/token_value_rule.hpp"
#include "../../core/tokenizer/tokens/integer_token.h"
#include "../../core/tokenizer/tokens/name_token.h"
#include "../../core/tokenizer/tokens/operator_token.h"

Grammar fe::GrammarFactory::Create() const {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::SequenceRule;
    using grammar_rules::TokenTypeRule;
    using grammar_rules::TokenValueRule;

    Grammar grammar;
    grammar.SetMainRule("program");

    grammar.AddRule("number", MakeRule<TokenTypeRule<IntegerToken>>());
    grammar.AddRule("identifier", MakeRule<TokenTypeRule<NameToken>>());

    grammar.AddRule("program",
                    MakeRule<SequenceRule>({MakeRule<NamedRule>("number"),
                                            MakeRule<TokenValueRule<OperatorToken>>("+"),
                                            MakeRule<NamedRule>("number")}));
    return grammar;
}
