#pragma once

#include <memory>

namespace syntax_tree {

class Node {
public:
    virtual ~Node() {
    }

private:
};

using NodePtr = std::shared_ptr<Node>;

}  // namespace syntax_tree