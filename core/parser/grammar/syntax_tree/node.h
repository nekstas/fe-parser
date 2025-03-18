#pragma once

#include <memory>
#include <vector>

namespace syntax_tree {

class Node;
using NodePtr = std::shared_ptr<Node>;

class Node {
public:
    virtual ~Node() {
    }

    virtual std::vector<NodePtr> GetChildren() const = 0;

    bool IsLeaf() const {
        return GetChildren().empty();
    }

private:
};

template <typename T, typename... Args>
NodePtr MakeNode(Args&&... args) {
    return std::make_shared<T>(args...);
};

}  // namespace syntax_tree