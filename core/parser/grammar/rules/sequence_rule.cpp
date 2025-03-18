#include "sequence_rule.h"

#include "../syntax_tree/sequence_node.h"

grammar_rules::SequenceRule::SequenceRule(std::initializer_list<GrammarRule> rules) : rules_(rules) {
}

syntax_tree::NodePtr grammar_rules::SequenceRule::Parse(TokensStream& tokens,
                                                        const Grammar& grammar) {
    std::vector<syntax_tree::NodePtr> result(rules_.size());
    for (size_t i = 0; i < rules_.size(); ++i) {
        result[i] = rules_[i]->Parse(tokens, grammar);
        if (!result[i]) {
            return syntax_tree::NodePtr();
        }
    }
    return MakeNode<syntax_tree::SequenceNode>(result);
}
