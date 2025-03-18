#pragma once

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class VariantNode : public Node {
public:
    VariantNode(size_t option, syntax_tree::NodePtr result);

public:
    size_t GetOption() const;

    syntax_tree::NodePtr GetResult() const;

    std::string ToString() const override;

private:
    size_t option_;
};

}  // namespace syntax_tree
