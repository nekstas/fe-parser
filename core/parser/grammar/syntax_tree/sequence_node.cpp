#include "sequence_node.h"

syntax_tree::SequenceNode::SequenceNode(const std::vector<syntax_tree::NodePtr>& sequence)
    : Node(sequence) {
}

std::string syntax_tree::SequenceNode::ToString() const {
    return "[SequenceNode]:";
}
