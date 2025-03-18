#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../../../utils/format_stream.h"

namespace syntax_tree {

class Node;
using NodePtr = std::shared_ptr<Node>;

class Node {
public:
    Node() = default;

    Node(const std::vector<NodePtr>& children) : children_(children) {
    }

    virtual ~Node() {
    }

    const std::vector<NodePtr>& GetChildren() const {
        return children_;
    }

    virtual std::string ToString() const = 0;

public:
    static std::string GetStringRepresentation(NodePtr node, const std::string& indent = "") {
        if (!node) {
            return indent + "[EmptyNode]";
        }

        FormatStream stream;
        stream << indent << node->ToString() << "\n";
        for (NodePtr another_node : node->GetChildren()) {
            stream << GetStringRepresentation(another_node, indent + "  ");
        }
        return stream;
    }

protected:
    std::vector<NodePtr> children_;
};

template <typename T, typename... Args>
NodePtr MakeNode(Args&&... args) {
    return std::make_shared<T>(args...);
};

}  // namespace syntax_tree