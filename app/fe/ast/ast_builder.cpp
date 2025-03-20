#include "ast_builder.h"

#include "../../../core/parser/grammar/syntax_tree/named_node.h"
#include "../../../core/parser/grammar/syntax_tree/variant_node.h"
#include "../../../core/tokenizer/tokens/integer_token.h"
#include "expressions/expression.h"
#include "expressions/number_expression.h"

AstBuilder::AstBuilder(const syntax_tree::NodePtr& root) : root_(root) {
}

ast::NodePtr AstBuilder::Build() {
    auto real_root = GetChild(root_, 0);

    return BuildExpression(real_root);
}

void AstBuilder::Require(bool condition, std::string message) {
    if (!condition) {
        throw AstBuilderError{message};
    }
}

syntax_tree::NodePtr AstBuilder::GetChild(syntax_tree::NodePtr root, size_t index) {
    Require(static_cast<bool>(root), "GetChild: root can't be null pointer.");
    Require(index < root->GetChildren().size(), "GetChild: there are no child with given index.");

    return root->GetChildren()[index];
}

syntax_tree::NodePtr AstBuilder::UnpackNamedNode(
    syntax_tree::NodePtr root, const std::string& name
) {
    auto node = syntax_tree::Cast<syntax_tree::NamedNode>(root);
    Require(node->GetName() == name, "UnpackNamedNode: root name and expected name are not equal.");

    return node->GetChildren()[0];
}

std::pair<size_t, syntax_tree::NodePtr> AstBuilder::UnpackVariantNode(syntax_tree::NodePtr root) {
    auto node = syntax_tree::Cast<syntax_tree::VariantNode>(root);

    return {node->GetOption(), node->GetChildren()[0]};
}

std::shared_ptr<ast::Expression> AstBuilder::BuildExpression(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "expression");
    return BuildBinaryExpression(node, 1);
}

std::shared_ptr<ast::Expression> AstBuilder::BuildBinaryExpression(
    syntax_tree::NodePtr root, size_t priority
) {
    std::string node_name = (FormatStream() << "expression_p" << priority);
    auto node = UnpackNamedNode(root, node_name);

    if (priority == kMaxExpressionPriority) {
        return BuildExpressionAtom(node);
    }

    auto first_expression = BuildBinaryExpression(GetChild(node, 0), priority + 1);
    return first_expression;
}

std::shared_ptr<ast::Expression> AstBuilder::BuildExpressionAtom(syntax_tree::NodePtr root) {
    auto [option, node] = UnpackVariantNode(root);
    // "number", "function_call", "variable", "brackets_expression", "unary_operation"
    Require(
        option < kExpressionAtomOptionsCount,
        "BuildExpressionAtom: Unknown option of atom of expression."
    );

    switch (option) {
        case 0:
            return BuildNumberExpression(node);
        case 1:
            return BuildFunctionCallExpression(node);
        case 2:
            return BuildVariableExpression(node);
        case 3:
            return BuildBracketsExpression(node);
        case 4:
            return BuildUnaryExpression(node);
    }

    throw std::logic_error{"Expected more case handlers of atom expression."};
}

std::shared_ptr<ast::Expression> AstBuilder::BuildNumberExpression(syntax_tree::NodePtr root) {
    auto node = ExtractToken<IntegerToken>(UnpackNamedNode(root, "number"));
    return ast::MakeNode<ast::NumberExpression>(node.GetValue());
}

std::shared_ptr<ast::Expression> AstBuilder::BuildFunctionCallExpression(syntax_tree::NodePtr root
) {
    return std::shared_ptr<ast::Expression>();
}

std::shared_ptr<ast::Expression> AstBuilder::BuildVariableExpression(syntax_tree::NodePtr root) {
    return std::shared_ptr<ast::Expression>();
}

std::shared_ptr<ast::Expression> AstBuilder::BuildBracketsExpression(syntax_tree::NodePtr root) {
    return std::shared_ptr<ast::Expression>();
}

std::shared_ptr<ast::Expression> AstBuilder::BuildUnaryExpression(syntax_tree::NodePtr root) {
    return std::shared_ptr<ast::Expression>();
}
