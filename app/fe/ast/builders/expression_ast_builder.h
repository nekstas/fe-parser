#pragma once

#include "basic_ast_builder.h"

namespace ast {

class ExpressionAstBuilder : public BasicAstBuilder {
protected:
    static constexpr size_t kMaxExpressionPriority = 4;

public:
    ExpressionAstBuilder();

protected:
    static std::string GetIdentifier(syntax_tree::NodePtr root);

    static std::string GetExtendedIdentifier(syntax_tree::NodePtr root);

    static std::string GetUnaryOperator(syntax_tree::NodePtr root);

    std::vector<std::string> GetIdentifierArgsList(syntax_tree::NodePtr root);

    std::vector<std::shared_ptr<ast::Expression>> GetExpressionArgsList(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> ConstructLeftBinaryExpression(
        const std::vector<std::shared_ptr<ast::Expression>>& expressions,
        const std::vector<std::string>& codes
    );

    std::shared_ptr<ast::Expression> ConstructRightBinaryExpression(
        const std::vector<std::shared_ptr<ast::Expression>>& expressions,
        const std::vector<std::string>& codes
    );

    ast::ExpressionsInfo::Info ParseBinaryExpressionParts(
        syntax_tree::NodePtr root, std::vector<std::shared_ptr<ast::Expression>>& expressions,
        std::vector<std::string>& codes
    );

    std::shared_ptr<ast::Expression> BuildExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildBinaryExpression(
        syntax_tree::NodePtr root, size_t priority
    );

    std::shared_ptr<ast::Expression> BuildAtomExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::NumberExpression> BuildNumberExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildFunctionCallExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::VariableExpression> BuildVariableExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildBracketsExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::NumberExpression> BuildUnaryExpression(syntax_tree::NodePtr root);

protected:
    ast::ExpressionsInfo expressions_info_;
};

}  // namespace ast
