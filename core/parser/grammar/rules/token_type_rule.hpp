#pragma once

#include "../syntax_tree/node.h"
#include "../syntax_tree/token_node.h"
#include "abstract_rule.h"

namespace grammar_rules {

template <typename TokenType>
class TokenTypeRule : public AbstractRule {
public:
    TokenTypeRule() = default;

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;
};

template <typename TokenType>
syntax_tree::NodePtr TokenTypeRule<TokenType>::Parse(TokensStream& tokens, const Grammar& grammar) {
    auto token = tokens.SafeGet();
    if (IsToken<TokenType>(token)) {
        return syntax_tree::MakeNode<syntax_tree::TokenNode>(token);
    }
    return {};
}

};  // namespace grammar_rules
