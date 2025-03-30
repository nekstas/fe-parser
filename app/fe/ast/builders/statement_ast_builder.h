#pragma once

#include "expression_ast_builder.h"

namespace ast {

class StatementAstBuilder : public ExpressionAstBuilder {
protected:
public:
    StatementAstBuilder() = default;

protected:
    std::shared_ptr<ast::Module> GetModuleFromWherePart(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Module> BuildModule(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Statement> BuildStatement(syntax_tree::NodePtr root);

    std::shared_ptr<ast::DefineVariableStatement> BuildDefineVariableStatement(
        syntax_tree::NodePtr root
    );

    std::shared_ptr<ast::DefineFunctionStatement> BuildDefineFunctionStatement(
        syntax_tree::NodePtr root
    );

    std::shared_ptr<ast::DefineModuleStatement> BuildDefineModuleStatement(
        syntax_tree::NodePtr root
    );

    std::shared_ptr<ast::ImportStatement> BuildImportStatement(syntax_tree::NodePtr root);

    std::shared_ptr<ast::ImportStatement> BuildImportAsStatement(
        const std::string& name, syntax_tree::NodePtr root
    );

    std::shared_ptr<ast::ImportStatement> BuildImportListStatement(
        const std::string& name, syntax_tree::NodePtr root
    );

protected:
};

}  // namespace ast
