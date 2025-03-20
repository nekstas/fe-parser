#pragma once

#include <exception>
#include <memory>
#include <stdexcept>

#include "fwd.h"

namespace ast {

class ImpossibleNodeCastError : public std::runtime_error {
public:
    ImpossibleNodeCastError() : std::runtime_error{"Impossible to cast ast node to another type."} {
    }
};

class Node {
public:
    virtual ~Node() {
    }

    virtual void Accept(Visitor& visitor) = 0;

private:
};

using NodePtr = std::shared_ptr<Node>;

template <typename T, typename... Args>
std::shared_ptr<T> MakeNode(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

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

}  // namespace ast
