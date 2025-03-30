#include "repeat_rule.h"

#include "../grammar.h"
#include "../syntax_tree/repeat_node.h"

grammar_rules::RepeatRule::RepeatRule(grammar_rules::GrammarRule rule, bool accept_empty)
    : rule_(rule), accept_empty_(accept_empty) {
    if (!rule_) {
        throw EmptyRuleError{"There is an empty rule for OptionalRule."};
    }
}
syntax_tree::NodePtr grammar_rules::RepeatRule::Parse(
    TokensStream& tokens, const Grammar& grammar
) {
    std::vector<syntax_tree::NodePtr> sequence;

    while (true) {
        size_t current_pos = tokens.GetPos();
        syntax_tree::NodePtr result = rule_->Parse(tokens, grammar);

        if (!result) {
            tokens.Restore(current_pos);
            break;
        }

        sequence.push_back(result);
    }

    if (!accept_empty_ && sequence.empty()) {
        return {};
    }

    return syntax_tree::MakeNode<syntax_tree::RepeatNode>(sequence);
}
