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
    virtual ~Node() {
    }

    virtual std::vector<NodePtr> GetChildren() const = 0;

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
};

template <typename T, typename... Args>
NodePtr MakeNode(Args&&... args) {
    return std::make_shared<T>(args...);
};

}  // namespace syntax_tree