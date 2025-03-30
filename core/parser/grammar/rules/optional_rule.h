#pragma once

#include "abstract_rule.h"

namespace grammar_rules {

class OptionalRule : public AbstractRule {
public:
    OptionalRule(GrammarRule rule);

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    GrammarRule rule_;
};

};  // namespace grammar_rules
