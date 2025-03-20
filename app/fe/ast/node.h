#pragma once

#include <memory>

#include "visitors/visitor.h"

namespace ast {

class Node {
public:
    virtual ~Node() {
    }

    virtual void Accept(Visitor& visitor) = 0;

private:
};

using NodePtr = std::shared_ptr<Node>;

}  // namespace ast
