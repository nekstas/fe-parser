#pragma once

#include "../rules/abstract_rule.h"
#include "node.h"

namespace syntax_tree {

class OptionalChildNodeBadAccessError : public std::runtime_error {
public:
    OptionalChildNodeBadAccessError()
        : std::runtime_error{
              "OptionalNode has not an inner node, but there was an attempt to get an inner "
              "node."} {
    }
};

class OptionalNode : public Node {
public:
    OptionalNode();

    OptionalNode(syntax_tree::NodePtr result);

public:
    bool HasNode() const;

    syntax_tree::NodePtr GetResult() const;

    std::string ToString() const override;

private:
};

}  // namespace syntax_tree
