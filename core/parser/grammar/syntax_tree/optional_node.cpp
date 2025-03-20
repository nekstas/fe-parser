#include "optional_node.h"

syntax_tree::OptionalNode::OptionalNode() : Node() {
}

syntax_tree::OptionalNode::OptionalNode(syntax_tree::NodePtr result) : Node({result}) {
}

bool syntax_tree::OptionalNode::HasNode() const {
    return !children_.empty();
}

syntax_tree::NodePtr syntax_tree::OptionalNode::GetResult() const {
    if (!HasNode()) {
        throw OptionalChildNodeBadAccessError{};
    }
    return children_.front();
}

std::string syntax_tree::OptionalNode::ToString() const {
    return FormatStream() << "[OptionalNode]: has_node=" << (HasNode() ? "true" : "false");
}
