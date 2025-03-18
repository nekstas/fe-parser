#include "token_node.h"

syntax_tree::TokenNode::TokenNode(Token token) : token_(token) {
}

std::vector<syntax_tree::NodePtr> syntax_tree::TokenNode::GetChildren() const {
    return {};
}

Token syntax_tree::TokenNode::GetToken() const {
    return token_;
}

std::string syntax_tree::TokenNode::ToString() const {
    return (FormatStream() << "[TokenNode]: (" << AbstractToken::GetStringRepresentation(token_)
                           << ")");
}
