#pragma once

#include "../expressions/expression.h"
#include "../statements/statement.h"
#include "statement_ast_builder.h"

namespace ast {

class FullAstBuilder : public StatementAstBuilder {
public:
    FullAstBuilder() = default;

    NodePtr Build(syntax_tree::NodePtr root) {
        auto real_root = GetChild(root, 0);

        return BuildStatement(real_root);
    }

protected:
};

}  // namespace ast
