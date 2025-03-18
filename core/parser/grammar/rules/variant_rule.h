#pragma once

#include "abstract_rule.h"

namespace grammar_rules {

class VariantRule : public AbstractRule {
public:
    VariantRule(std::vector<GrammarRule> rules);

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    std::vector<GrammarRule> rules_;
};

};  // namespace grammar_rules
