#include "variant_node.h"

syntax_tree::VariantNode::VariantNode(size_t option, syntax_tree::NodePtr result)
    : Node({result}), option_(option) {
}

size_t syntax_tree::VariantNode::GetOption() const {
    return option_;
}

syntax_tree::NodePtr syntax_tree::VariantNode::GetResult() const {
    return children_.front();
}

std::string syntax_tree::VariantNode::ToString() const {
    return FormatStream() << "[VariantNode]: option=" << option_;
}
