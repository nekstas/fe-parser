#pragma once

#include <vector>

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class SequenceNode : public Node {
public:
    SequenceNode(const std::vector<syntax_tree::NodePtr>& sequence);

public:
    std::string ToString() const override;
};

}  // namespace syntax_tree
