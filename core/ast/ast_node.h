#pragma once

#include "ast_visitor.h"

namespace ast {

class AstNode {
public:
    virtual ~AstNode() {
    }

    virtual void Accept(AstVisitor& visitor) = 0;

private:
};

}  // namespace ast
