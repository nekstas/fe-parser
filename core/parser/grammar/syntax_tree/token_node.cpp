#include "token_node.h"

syntax_tree::TokenNode::TokenNode(Token token) : token_(token) {
}

Token syntax_tree::TokenNode::GetToken() const {
    return token_;
}

std::string syntax_tree::TokenNode::ToString() const {
    return FormatStream() << "[TokenNode]: token=\""
                          << AbstractToken::GetStringRepresentation(token_) << "\"";
}
