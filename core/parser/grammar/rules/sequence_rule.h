#pragma once

#include "abstract_rule.h"

namespace grammar_rules {

class SequenceRule : public AbstractRule {
public:
    SequenceRule(std::initializer_list<GrammarRule> rules);

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    std::vector<GrammarRule> rules_;
};

};  // namespace grammar_rules
