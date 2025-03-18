#pragma once

#include "../syntax_tree/node.h"
#include "../syntax_tree/token_node.h"
#include "abstract_rule.h"

namespace grammar_rules {

template <typename TokenType>
class TokenValueRule : public AbstractRule {
public:
    template <typename... Args>
    TokenValueRule(Args&&... args) : token_(TokenType(std::forward<Args>(args)...)){};

    syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) override;

private:
    TokenType token_;
};

template <typename TokenType>
syntax_tree::NodePtr TokenValueRule<TokenType>::Parse(TokensStream& tokens,
                                                      const Grammar& grammar) {
    auto token = ConvertTokenTo<TokenType>(tokens.Get());
    if (!token || *token != token_) {
        return {};
    }

    return syntax_tree::MakeNode<syntax_tree::TokenNode>(token);
}

};  // namespace grammar_rules
