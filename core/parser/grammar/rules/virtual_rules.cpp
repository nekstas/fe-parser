#include "virtual_rules.h"

#include "../../../tokenizer/tokens/close_bracket_token.h"
#include "../../../tokenizer/tokens/name_token.h"
#include "../../../tokenizer/tokens/new_line_token.h"
#include "../../../tokenizer/tokens/open_bracket_token.h"
#include "../../../tokenizer/tokens/operator_token.h"
#include "named_rule.h"
#include "repeat_rule.h"
#include "sequence_rule.h"
#include "token_type_rule.hpp"
#include "token_value_rule.hpp"
#include "variant_rule.h"

grammar_rules::GrammarRule grammar_rules::VirtualRules::SeparatorRule(
    grammar_rules::GrammarRule element, grammar_rules::GrammarRule separator, bool accept_empty
) {
    return MakeRule<SequenceRule>(
        {element, MakeRule<RepeatRule>(MakeRule<SequenceRule>({separator, element}), accept_empty)}
    );
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::KeywordRule(const std::string& name) {
    return MakeRule<TokenValueRule<NameToken>>(name);
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::OperatorRule(const std::string& code) {
    return MakeRule<TokenValueRule<OperatorToken>>(code);
}
grammar_rules::GrammarRule grammar_rules::VirtualRules::Choice(
    std::initializer_list<std::string> names
) {
    std::vector<GrammarRule> rules;
    for (const std::string& name : names) {
        rules.push_back(MakeRule<NamedRule>(name));
    }
    return MakeRule<VariantRule>(rules);
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::OpenBracket() {
    return MakeRule<TokenTypeRule<OpenBracketToken>>();
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::CloseBracket() {
    return MakeRule<TokenTypeRule<CloseBracketToken>>();
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::NewLine() {
    return MakeRule<TokenTypeRule<NewLineToken>>();
}
