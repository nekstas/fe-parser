#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../../../utils/format_stream.h"

namespace syntax_tree {

class Node;
using NodePtr = std::shared_ptr<Node>;

class ImpossibleNodeCastError : public std::runtime_error {
public:
    ImpossibleNodeCastError()
        : std::runtime_error{"Impossible to cast syntax tree node to another type."} {
    }
};

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
            stream << GetStringRepresentation(another_node, indent + " ");
        }
        return stream;
    }

protected:
    std::vector<NodePtr> children_;
};

template <typename T, typename... Args>
NodePtr MakeNode(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
};

template <typename T>
std::shared_ptr<T> To(NodePtr node) {
    return std::dynamic_pointer_cast<T>(node);
}

template <typename T>
std::shared_ptr<T> Cast(NodePtr node) {
    auto result = To<T>(node);
    if (!result) {
        throw ImpossibleNodeCastError{};
    }
    return result;
}

template <typename T>
bool Is(NodePtr token) {
    return static_cast<bool>(To<T>(token));
}

}  // namespace syntax_tree