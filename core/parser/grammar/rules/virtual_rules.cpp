#include "virtual_rules.h"

#include "../../../tokenizer/tokens/name_token.h"
#include "../../../tokenizer/tokens/operator_token.h"
#include "repeat_rule.h"
#include "sequence_rule.h"
#include "token_value_rule.hpp"

grammar_rules::GrammarRule grammar_rules::VirtualRules::SeparatorRule(
    grammar_rules::GrammarRule element, grammar_rules::GrammarRule separator, bool accept_empty) {
    return MakeRule<SequenceRule>(
        {element,
         MakeRule<RepeatRule>(MakeRule<SequenceRule>({separator, element}), accept_empty)});
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::KeywordRule(const std::string& name) {
    return MakeRule<TokenValueRule<NameToken>>(name);
}

grammar_rules::GrammarRule grammar_rules::VirtualRules::OperatorRule(const std::string& code) {
    return MakeRule<TokenValueRule<OperatorToken>>(code);
}
