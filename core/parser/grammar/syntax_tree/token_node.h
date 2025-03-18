#pragma once

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class TokenNode : public Node {
public:
    TokenNode(Token token);

public:
    std::vector<NodePtr> GetChildren() const override;

    std::string ToString() const override;

    Token GetToken() const;

private:
    Token token_;
};

}  // namespace syntax_tree
