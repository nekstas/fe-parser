#include "repeat_node.h"

syntax_tree::RepeatNode::RepeatNode(const std::vector<syntax_tree::NodePtr>& result)
    : Node({result}) {
}

size_t syntax_tree::RepeatNode::GetNodesCount() const {
    return children_.size();
}

std::string syntax_tree::RepeatNode::ToString() const {
    return FormatStream() << "[RepeatNode]: nodes_count=" << GetNodesCount();
}
