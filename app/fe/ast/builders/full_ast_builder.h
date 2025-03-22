#pragma once

#include "../module.h"
#include "statement_ast_builder.h"

namespace ast {

class FullAstBuilder : public StatementAstBuilder {
public:
    FullAstBuilder() = default;

    NodePtr Build(syntax_tree::NodePtr root) {
        return BuildModule(root);
    }

protected:
};

}  // namespace ast
