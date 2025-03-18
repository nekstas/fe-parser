#pragma once

#include "abstract_rule.h"

namespace grammar_rules {

class RepeatRule : public AbstractRule {
public:
    RepeatRule(GrammarRule rule, bool accept_empty = false);

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    GrammarRule rule_;
    bool accept_empty_;
};

};  // namespace grammar_rules
