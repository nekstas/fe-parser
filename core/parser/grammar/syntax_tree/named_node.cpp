#include "named_node.h"

syntax_tree::NamedNode::NamedNode(const std::string& name, syntax_tree::NodePtr result)
    : Node({result}), name_(name) {
}

std::string syntax_tree::NamedNode::ToString() const {
    return FormatStream() << "[NamedNode]: name=\"" << name_ << "\"";
}

std::string syntax_tree::NamedNode::GetName() const {
    return name_;
}
