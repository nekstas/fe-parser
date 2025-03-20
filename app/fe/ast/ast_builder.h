#pragma once

#include <cstdint>

#include "../../../core/parser/grammar/syntax_tree/node.h"
#include "../../../core/parser/grammar/syntax_tree/token_node.h"
#include "node.h"

class AstBuilderError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class AstBuilder {
public:
    static constexpr size_t kMaxExpressionPriority = 4;
    static constexpr size_t kExpressionAtomOptionsCount = 5;

public:
    AstBuilder(const syntax_tree::NodePtr& root);

    ast::NodePtr Build();

private:
    static void Require(bool condition, std::string message);

    static syntax_tree::NodePtr GetChild(syntax_tree::NodePtr root, size_t index);

    static syntax_tree::NodePtr UnpackNamedNode(syntax_tree::NodePtr root, const std::string& name);

    static std::pair<size_t, syntax_tree::NodePtr> UnpackVariantNode(syntax_tree::NodePtr root);

    template <typename T>
    static const T& ExtractToken(syntax_tree::NodePtr root) {
        auto node = syntax_tree::Cast<syntax_tree::TokenNode>(root);
        auto token = CastToken<T>(node->GetToken());
        return *token;
    }

    std::shared_ptr<ast::Expression> BuildExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildBinaryExpression(
        syntax_tree::NodePtr root, size_t priority
    );

    std::shared_ptr<ast::Expression> BuildExpressionAtom(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildNumberExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildFunctionCallExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildVariableExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildBracketsExpression(syntax_tree::NodePtr root);

    std::shared_ptr<ast::Expression> BuildUnaryExpression(syntax_tree::NodePtr root);

private:
    syntax_tree::NodePtr root_;
};