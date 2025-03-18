#pragma once

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class NamedNode : public Node {
public:
    NamedNode(const std::string& name, NodePtr result);

public:
    std::string ToString() const override;

    std::string GetName() const;

private:
    std::string name_;
};

}  // namespace syntax_tree
