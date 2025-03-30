#include "optional_rule.h"

#include "../grammar.h"
#include "../syntax_tree/optional_node.h"

grammar_rules::OptionalRule::OptionalRule(grammar_rules::GrammarRule rule) : rule_(rule) {
    if (!rule_) {
        throw EmptyRuleError{"There is an empty rule for OptionalRule."};
    }
}

syntax_tree::NodePtr grammar_rules::OptionalRule::Parse(
    TokensStream& tokens, const Grammar& grammar
) {
    size_t current_pos = tokens.GetPos();
    syntax_tree::NodePtr result = rule_->Parse(tokens, grammar);
    if (result) {
        return syntax_tree::MakeNode<syntax_tree::OptionalNode>(result);
    }

    tokens.Restore(current_pos);
    return syntax_tree::MakeNode<syntax_tree::OptionalNode>();
}
