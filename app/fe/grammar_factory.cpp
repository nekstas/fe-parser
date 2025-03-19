#include "grammar_factory.h"

#include "../../core/parser/grammar/rules/named_rule.h"
#include "../../core/parser/grammar/rules/optional_rule.h"
#include "../../core/parser/grammar/rules/repeat_rule.h"
#include "../../core/parser/grammar/rules/sequence_rule.h"
#include "../../core/parser/grammar/rules/token_type_rule.hpp"
#include "../../core/parser/grammar/rules/token_value_rule.hpp"
#include "../../core/parser/grammar/rules/variant_rule.h"
#include "../../core/parser/grammar/rules/virtual_rules.h"
#include "../../core/tokenizer/tokens/integer_token.h"
#include "../../core/tokenizer/tokens/name_token.h"
#include "../../core/tokenizer/tokens/new_line_token.h"

Grammar fe::GrammarFactory::Create() const {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::OptionalRule;
    using grammar_rules::RepeatRule;
    using grammar_rules::SequenceRule;
    using grammar_rules::TokenTypeRule;
    using grammar_rules::TokenValueRule;
    using grammar_rules::VariantRule;
    using grammar_rules::VirtualRules;

    Grammar grammar;

    grammar.AddRule("number", MakeRule<TokenTypeRule<IntegerToken>>());
    grammar.AddRule("identifier", MakeRule<TokenTypeRule<NameToken>>());

    grammar.AddRule(
        "extended_identifier",
        VirtualRules::SeparatorRule(
            MakeRule<NamedRule>("identifier"), VirtualRules::OperatorRule("."), true
        )
    );

    grammar.AddRule(
        "identifier_args_list",
        MakeRule<SequenceRule>(
            {VirtualRules::OperatorRule("("),
             MakeRule<OptionalRule>(VirtualRules::SeparatorRule(
                 MakeRule<NamedRule>("identifier"), VirtualRules::OperatorRule(","), true
             )),
             VirtualRules::OperatorRule(")")}
        )
    );
    grammar.AddRule(
        "expression_args_list",
        MakeRule<SequenceRule>(
            {VirtualRules::OperatorRule("("),
             MakeRule<OptionalRule>(VirtualRules::SeparatorRule(
                 MakeRule<NamedRule>("expression"), VirtualRules::OperatorRule(","), true
             )),
             VirtualRules::OperatorRule(")")}
        )
    );

    grammar.AddRule("variable", MakeRule<NamedRule>("extended_identifier"));
    grammar.AddRule(
        "function_call", MakeRule<SequenceRule>(
                             {MakeRule<NamedRule>("extended_identifier"),
                              MakeRule<NamedRule>("expression_args_list")}
                         )
    );
    grammar.AddRule(
        "brackets_expression",
        MakeRule<SequenceRule>(
            {VirtualRules::OperatorRule("("), MakeRule<NamedRule>("expression"),
             VirtualRules::OperatorRule(")")}
        )
    );
    grammar.AddRule(
        "unary_operator",
        MakeRule<VariantRule>({VirtualRules::OperatorRule("+"), VirtualRules::OperatorRule("-")})
    );
    grammar.AddRule(
        "unary_operation",
        MakeRule<SequenceRule>(
            {MakeRule<NamedRule>("unary_operator"), MakeRule<NamedRule>("number")}
        )
    );
    grammar.AddRule(
        "expression_p4", MakeRule<VariantRule>({
                             MakeRule<NamedRule>("number"),
                             MakeRule<NamedRule>("function_call"),
                             MakeRule<NamedRule>("variable"),
                             MakeRule<NamedRule>("brackets_expression"),
                             MakeRule<NamedRule>("unary_operation"),
                         })
    );

    grammar.AddRule("expression", MakeRule<NamedRule>("expression_p4"));

    grammar.AddRule(
        "program", MakeRule<SequenceRule>(
                       {MakeRule<NamedRule>("expression"), MakeRule<TokenTypeRule<NewLineToken>>()}
                   )
    );

    grammar.SetMainRule("program");
    return grammar;
}
