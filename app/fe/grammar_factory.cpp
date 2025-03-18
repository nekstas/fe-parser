#include "grammar_factory.h"

#include "../../core/parser/grammar/rules/named_rule.h"
#include "../../core/parser/grammar/rules/sequence_rule.h"
#include "../../core/parser/grammar/rules/token_type_rule.hpp"
#include "../../core/parser/grammar/rules/token_value_rule.hpp"
#include "../../core/parser/grammar/rules/variant_rule.h"
#include "../../core/tokenizer/tokens/integer_token.h"
#include "../../core/tokenizer/tokens/name_token.h"
#include "../../core/tokenizer/tokens/operator_token.h"

Grammar fe::GrammarFactory::Create() const {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::SequenceRule;
    using grammar_rules::TokenTypeRule;
    using grammar_rules::TokenValueRule;
    using grammar_rules::VariantRule;

    Grammar grammar;

    grammar.AddRule("number", MakeRule<TokenTypeRule<IntegerToken>>());
    grammar.AddRule("identifier", MakeRule<TokenTypeRule<NameToken>>());

    grammar.AddRule("expression", MakeRule<SequenceRule>({
                                      MakeRule<NamedRule>("atom"),
                                      MakeRule<NamedRule>("operator"),
                                      MakeRule<NamedRule>("atom"),
                                  }));

    grammar.AddRule("atom", MakeRule<VariantRule>({
                                MakeRule<TokenTypeRule<IntegerToken>>(),
                                MakeRule<TokenTypeRule<NameToken>>(),
                            }));

    grammar.AddRule("operator", MakeRule<VariantRule>({
                                    MakeRule<TokenValueRule<OperatorToken>>("+"),
                                    MakeRule<TokenValueRule<OperatorToken>>("-"),
                                }));

    grammar.AddRule("program", MakeRule<NamedRule>("expression"));

    grammar.SetMainRule("program");
    return grammar;
}
