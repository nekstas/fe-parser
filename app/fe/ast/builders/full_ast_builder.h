#pragma once

#include "../expressions/expression.h"
#include "expression_ast_builder.h"

namespace ast {

class FullAstBuilder : public ExpressionAstBuilder {
public:
    FullAstBuilder() = default;

    NodePtr Build(syntax_tree::NodePtr root) {
        auto real_root = GetChild(root, 0);

        return BuildExpression(real_root);
    }

protected:
};

}  // namespace ast
