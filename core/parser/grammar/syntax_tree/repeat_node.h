#pragma once

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class RepeatNode : public Node {
public:
    RepeatNode(const std::vector<syntax_tree::NodePtr>& result);

public:
    size_t GetNodesCount() const;

    std::string ToString() const override;

private:
};

}  // namespace syntax_tree

