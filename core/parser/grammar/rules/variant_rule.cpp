#include "variant_rule.h"

#include "../syntax_tree/variant_node.h"

grammar_rules::VariantRule::VariantRule(std::vector<GrammarRule> rules) : rules_(rules) {
}

syntax_tree::NodePtr grammar_rules::VariantRule::Parse(TokensStream& tokens,
                                                       const Grammar& grammar) {
    size_t current_pos = tokens.GetPos();
    for (size_t i = 0; i < rules_.size(); ++i) {
        syntax_tree::NodePtr result = rules_[i]->Parse(tokens, grammar);
        if (result) {
            return MakeNode<syntax_tree::VariantNode>(i, result);
        }
        tokens.Restore(current_pos);
    }
    return {};
}
