#pragma once

#include <string>

#include "abstract_rule.h"

namespace grammar_rules {

class NamedRule : public AbstractRule {
public:
    NamedRule(const std::string& name);

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    std::string name_;
};

};  // namespace grammar_rules
