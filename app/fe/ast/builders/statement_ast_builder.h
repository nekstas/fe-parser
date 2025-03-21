#pragma once

#include "expression_ast_builder.h"

namespace ast {

class StatementAstBuilder : public ExpressionAstBuilder {
protected:
public:
    StatementAstBuilder() = default;

protected:
    std::shared_ptr<ast::Statement> BuildStatement(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Statement> BuildDefineVariableStatement(syntax_tree::NodePtr root);

protected:
};

}  // namespace ast
