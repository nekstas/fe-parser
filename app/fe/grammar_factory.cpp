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
        "expression_p4",
        VirtualRules::Choice(
            {"number", "function_call", "variable", "brackets_expression", "unary_operation"}
        )
    );

    grammar.AddRule(
        "expression_p3",
        VirtualRules::SeparatorRule(
            MakeRule<NamedRule>("expression_p4"), VirtualRules::OperatorRule("^"), true
        )
    );

    grammar.AddRule(
        "expression_p2",
        VirtualRules::SeparatorRule(
            MakeRule<NamedRule>("expression_p3"),
            MakeRule<VariantRule>({VirtualRules::OperatorRule("*"), VirtualRules::OperatorRule("/")}
            ),
            true
        )
    );

    grammar.AddRule(
        "expression_p1",
        VirtualRules::SeparatorRule(
            MakeRule<NamedRule>("expression_p2"),
            MakeRule<VariantRule>({VirtualRules::OperatorRule("+"), VirtualRules::OperatorRule("-")}
            ),
            true
        )
    );

    grammar.AddRule("expression", MakeRule<NamedRule>("expression_p1"));

    grammar.AddRule(
        "where_part", MakeRule<SequenceRule>({
                          VirtualRules::KeywordRule("where"),
                          VirtualRules::OpenBracket(),
                          MakeRule<NamedRule>("module"),
                          VirtualRules::CloseBracket(),
                      })
    );
    grammar.AddRule(
        "as_part",
        MakeRule<SequenceRule>({VirtualRules::KeywordRule("as"), MakeRule<NamedRule>("identifier")})
    );

    grammar.AddRule(
        "import_statement",
        MakeRule<SequenceRule>(
            {VirtualRules::KeywordRule("import"), MakeRule<NamedRule>("extended_identifier"),
             MakeRule<OptionalRule>(VirtualRules::Choice({
                 "as_part",
                 "identifier_args_list",
             }))}
        )
    );

    grammar.AddRule(
        "define_variable_statement", MakeRule<SequenceRule>({
                                         VirtualRules::KeywordRule("let"),
                                         MakeRule<NamedRule>("identifier"),
                                         VirtualRules::OperatorRule(":="),
                                         MakeRule<NamedRule>("expression"),
                                     })
    );

    grammar.AddRule(
        "define_function_statement",
        MakeRule<SequenceRule>(
            {VirtualRules::KeywordRule("let"), MakeRule<NamedRule>("identifier"),
             MakeRule<NamedRule>("identifier_args_list"), VirtualRules::OperatorRule(":="),
             MakeRule<NamedRule>("expression"),
             MakeRule<OptionalRule>(MakeRule<NamedRule>("where_part"))}
        )
    );

    grammar.AddRule(
        "define_module_statement",
        MakeRule<SequenceRule>(
            {VirtualRules::KeywordRule("module"), MakeRule<NamedRule>("identifier"),
             MakeRule<NamedRule>("where_part")}
        )
    );

    grammar.AddRule(
        "statement", VirtualRules::Choice(
                         {"import_statement", "define_variable_statement",
                          "define_function_statement", "define_module_statement"}
                     )
    );

    grammar.AddRule(
        "module",
        MakeRule<RepeatRule>(
            MakeRule<SequenceRule>(
                {MakeRule<NamedRule>("statement"), VirtualRules::NewLine()}
            ),
            true
        )
    );

    grammar.AddRule("program", MakeRule<NamedRule>("module"));

    grammar.SetMainRule("program");
    return grammar;
}
