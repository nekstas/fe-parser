#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
    static constexpr std::string kExtendedIdentifierSeparator = ".";

public:
    AstBuilder(const syntax_tree::NodePtr& root);

    ast::NodePtr Build();

private:
    static void Require(bool condition, std::string message);

    static syntax_tree::NodePtr GetChild(syntax_tree::NodePtr root, size_t index);

    static syntax_tree::NodePtr UnpackNamedNode(syntax_tree::NodePtr root, const std::string& name);

    static std::pair<size_t, syntax_tree::NodePtr> UnpackVariantNode(syntax_tree::NodePtr root);

    static std::pair<size_t, std::vector<syntax_tree::NodePtr>> UnpackRepeatNode(
        syntax_tree::NodePtr root
    );

    static syntax_tree::NodePtr UnpackOptionalNode(syntax_tree::NodePtr root);

    template <typename T>
    static const T& ExtractToken(syntax_tree::NodePtr root) {
        auto node = syntax_tree::Cast<syntax_tree::TokenNode>(root);
        auto token = CastToken<T>(node->GetToken());
        return *token;
    }

    static std::string GetIdentifier(syntax_tree::NodePtr root);

    static std::string GetExtendedIdentifier(syntax_tree::NodePtr root);

    static std::string GetUnaryOperator(syntax_tree::NodePtr root);

    static std::vector<std::shared_ptr<ast::Expression>> GetExpressionArgsList(
        syntax_tree::NodePtr root
    );

    static std::shared_ptr<ast::Expression> BuildExpression(syntax_tree::NodePtr root);

    static std::shared_ptr<ast::Expression> BuildBinaryExpression(
        syntax_tree::NodePtr root, size_t priority
    );

    static std::shared_ptr<ast::Expression> BuildExpressionAtom(syntax_tree::NodePtr root);

    static std::shared_ptr<ast::NumberExpression> BuildNumberExpression(syntax_tree::NodePtr root);

    static std::shared_ptr<ast::Expression> BuildFunctionCallExpression(syntax_tree::NodePtr root);

    static std::shared_ptr<ast::VariableExpression> BuildVariableExpression(
        syntax_tree::NodePtr root
    );

    static std::shared_ptr<ast::Expression> BuildBracketsExpression(syntax_tree::NodePtr root);

    static std::shared_ptr<ast::NumberExpression> BuildUnaryExpression(syntax_tree::NodePtr root);

private:
    syntax_tree::NodePtr root_;
};