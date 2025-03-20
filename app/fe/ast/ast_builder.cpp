#include "ast_builder.h"

#include "../../../core/parser/grammar/syntax_tree/named_node.h"
#include "../../../core/parser/grammar/syntax_tree/optional_node.h"
#include "../../../core/parser/grammar/syntax_tree/repeat_node.h"
#include "../../../core/parser/grammar/syntax_tree/variant_node.h"
#include "../../../core/tokenizer/tokens/integer_token.h"
#include "../../../core/tokenizer/tokens/name_token.h"
#include "../../../core/tokenizer/tokens/operator_token.h"
#include "../../../utils/utils.h"
#include "expressions/call_expression.h"
#include "expressions/expression.h"
#include "expressions/number_expression.h"
#include "expressions/variable_expression.h"

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

std::pair<size_t, std::vector<syntax_tree::NodePtr>> AstBuilder::UnpackRepeatNode(
    syntax_tree::NodePtr root
) {
    auto node = syntax_tree::Cast<syntax_tree::RepeatNode>(root);

    return {node->GetNodesCount(), node->GetChildren()};
}

syntax_tree::NodePtr AstBuilder::UnpackOptionalNode(syntax_tree::NodePtr root) {
    auto node = syntax_tree::Cast<syntax_tree::OptionalNode>(root);

    if (node->HasNode()) {
        return node->GetResult();
    }
    return {};
}

std::string AstBuilder::GetIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "identifier");
    auto token = ExtractToken<NameToken>(node);
    return token.GetName();
}

std::string AstBuilder::GetExtendedIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "extended_identifier");

    std::vector<std::string> identifiers = {GetIdentifier(GetChild(node, 0))};
    auto [count, sequence] = UnpackRepeatNode(GetChild(node, 1));

    for (size_t i = 0; i < count; ++i) {
        auto child = sequence[i];
        identifiers.push_back(GetIdentifier(GetChild(child, 1)));
    }

    return utils::Join(identifiers, kExtendedIdentifierSeparator);
}

std::string AstBuilder::GetUnaryOperator(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "unary_operator");
    auto [_, token_node] = UnpackVariantNode(node);
    auto token = ExtractToken<OperatorToken>(token_node);
    return token.GetCode();
}

std::vector<std::shared_ptr<ast::Expression>> AstBuilder::GetExpressionArgsList(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "expression_args_list");
    auto list = UnpackOptionalNode(GetChild(node, 1));

    if (!list) {
        return {};
    }

    std::vector<std::shared_ptr<ast::Expression>> args = {BuildExpression(GetChild(list, 0))};
    auto [count, sequence] = UnpackRepeatNode(GetChild(list, 1));

    for (size_t i = 0; i < count; ++i) {
        auto child = sequence[i];
        args.push_back(BuildExpression(GetChild(child, 1)));
    }

    return args;
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

std::shared_ptr<ast::NumberExpression> AstBuilder::BuildNumberExpression(syntax_tree::NodePtr root
) {
    auto token = ExtractToken<IntegerToken>(UnpackNamedNode(root, "number"));
    return ast::MakeNode<ast::NumberExpression>(token.GetValue());
}

std::shared_ptr<ast::Expression> AstBuilder::BuildFunctionCallExpression(syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "function_call");
    auto callee = BuildVariableExpression(GetChild(node, 0));
    auto args = GetExpressionArgsList(GetChild(node, 1));

    return ast::MakeNode<ast::CallExpression>(callee, args);
}

std::shared_ptr<ast::VariableExpression> AstBuilder::BuildVariableExpression(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "variable");
    return ast::MakeNode<ast::VariableExpression>(GetExtendedIdentifier(node));
}

std::shared_ptr<ast::Expression> AstBuilder::BuildBracketsExpression(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "brackets_expression");
    auto expr_node = GetChild(node, 1);
    return BuildExpression(expr_node);
}

std::shared_ptr<ast::NumberExpression> AstBuilder::BuildUnaryExpression(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "unary_operation");
    auto unary_operator = GetUnaryOperator(GetChild(node, 0));
    auto number = BuildNumberExpression(GetChild(node, 1));

    Require(unary_operator == "+" || unary_operator == "-", "Unknown unary operator.");

    if (unary_operator == "+") {
        return number;
    } else {
        return ast::MakeNode<ast::NumberExpression>("-" + number->GetValue());
    }
}
