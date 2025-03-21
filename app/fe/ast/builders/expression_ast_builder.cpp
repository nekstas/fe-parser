#include "expression_ast_builder.h"

#include "../../../../core/tokenizer/tokens/integer_token.h"
#include "../../../../core/tokenizer/tokens/name_token.h"
#include "../../../../core/tokenizer/tokens/operator_token.h"
#include "../../../../utils/utils.h"
#include "../../factories/expressions_info_factory.h"
#include "../expressions/binary_expression.h"
#include "../expressions/call_expression.h"
#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"

ast::ExpressionAstBuilder::ExpressionAstBuilder()
    : expressions_info_(fe::ExpressionsInfoFactory().Create()) {
}

std::string ast::ExpressionAstBuilder::GetIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "identifier");
    auto token = ExtractToken<NameToken>(node);
    return token.GetName();
}

std::string ast::ExpressionAstBuilder::GetExtendedIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "extended_identifier");

    std::vector<std::string> identifiers = {GetIdentifier(GetChild(node, 0))};
    auto [count, sequence] = UnpackRepeatNode(GetChild(node, 1));

    for (size_t i = 0; i < count; ++i) {
        auto child = sequence[i];
        identifiers.push_back(GetIdentifier(GetChild(child, 1)));
    }

    return utils::Join(identifiers, kExtendedIdentifierSeparator);
}

std::string ast::ExpressionAstBuilder::GetUnaryOperator(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "unary_operator");
    auto [_, token_node] = UnpackVariantNode(node);
    auto token = ExtractToken<OperatorToken>(token_node);
    return token.GetCode();
}

std::vector<std::string> ast::ExpressionAstBuilder::GetIdentifierArgsList(syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "identifier_args_list");
    auto list = UnpackOptionalNode(GetChild(node, 1));

    if (!list) {
        return {};
    }

    std::vector<std::string> args = {GetIdentifier(GetChild(list, 0))};
    auto [count, sequence] = UnpackRepeatNode(GetChild(list, 1));

    for (size_t i = 0; i < count; ++i) {
        args.push_back(GetIdentifier(GetChild(sequence[i], 1)));
    }

    return args;
}

std::vector<std::shared_ptr<ast::Expression>> ast::ExpressionAstBuilder::GetExpressionArgsList(
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
        args.push_back(BuildExpression(GetChild(sequence[i], 1)));
    }

    return args;
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::ConstructLeftBinaryExpression(
    const std::vector<std::shared_ptr<ast::Expression>>& expressions,
    const std::vector<std::string>& codes
) {
    std::shared_ptr<ast::Expression> result = expressions.front();

    for (size_t i = 0; i < codes.size(); ++i) {
        result = ast::MakeNode<ast::BinaryExpression>(codes[i], result, expressions[i + 1]);
    }

    return result;
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::ConstructRightBinaryExpression(
    const std::vector<std::shared_ptr<ast::Expression>>& expressions,
    const std::vector<std::string>& codes
) {
    std::shared_ptr<ast::Expression> result = expressions.back();

    for (size_t i = codes.size(); i > 0; --i) {
        result = ast::MakeNode<ast::BinaryExpression>(codes[i - 1], expressions[i - 1], result);
    }

    return result;
}

ast::ExpressionsInfo::Info ast::ExpressionAstBuilder::ParseBinaryExpressionParts(
    syntax_tree::NodePtr root, std::vector<std::shared_ptr<ast::Expression>>& expressions,
    std::vector<std::string>& codes
) {
    auto [count, sequence] = UnpackRepeatNode(root);
    ast::ExpressionsInfo::Info info;

    for (size_t i = 0; i < count; ++i) {
        auto child = sequence[i];
        auto code_token = ExtractToken<OperatorToken>(UnpackVariantNode(GetChild(child, 0)).second);
        auto code = code_token.GetCode();

        codes.push_back(code);
        info = info.Merge(expressions_info_.GetInfo(code));

        expressions.push_back(BuildBinaryExpression(GetChild(child, 1), info.priority + 1));
    }

    return info;
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::BuildExpression(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "expression");
    return BuildBinaryExpression(node, 1);
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::BuildBinaryExpression(
    syntax_tree::NodePtr root, size_t priority
) {
    std::string node_name = (FormatStream() << "expression_p" << priority);
    auto node = UnpackNamedNode(root, node_name);

    if (priority == kMaxExpressionPriority) {
        return BuildAtomExpression(node);
    }

    std::vector<std::shared_ptr<ast::Expression>> expressions = {
        BuildBinaryExpression(GetChild(node, 0), priority + 1)
    };
    std::vector<std::string> codes;

    ast::ExpressionsInfo::Info info =
        ParseBinaryExpressionParts(GetChild(node, 1), expressions, codes);

    switch (info.associativity) {
        case ast::ExpressionsInfo::Associativity::LEFT:
        case ast::ExpressionsInfo::Associativity::BOTH:
            return ConstructLeftBinaryExpression(expressions, codes);
        case ast::ExpressionsInfo::Associativity::RIGHT:
            return ConstructRightBinaryExpression(expressions, codes);
        default:
            return expressions.front();
    }
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::BuildAtomExpression(
    syntax_tree::NodePtr root
) {
    auto [option, node] = UnpackVariantNode(root);

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

    throw AstBuilderError{"Expected more case handlers of atom expression."};
}

std::shared_ptr<ast::NumberExpression> ast::ExpressionAstBuilder::BuildNumberExpression(
    syntax_tree::NodePtr root
) {
    auto token = ExtractToken<IntegerToken>(UnpackNamedNode(root, "number"));
    return ast::MakeNode<ast::NumberExpression>(token.GetValue());
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::BuildFunctionCallExpression(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "function_call");
    auto callee = BuildVariableExpression(GetChild(node, 0));
    auto args = GetExpressionArgsList(GetChild(node, 1));

    return ast::MakeNode<ast::CallExpression>(callee, args);
}

std::shared_ptr<ast::VariableExpression> ast::ExpressionAstBuilder::BuildVariableExpression(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "variable");
    return ast::MakeNode<ast::VariableExpression>(GetExtendedIdentifier(node));
}

std::shared_ptr<ast::Expression> ast::ExpressionAstBuilder::BuildBracketsExpression(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "brackets_expression");
    auto expr_node = GetChild(node, 1);
    return BuildExpression(expr_node);
}

std::shared_ptr<ast::NumberExpression> ast::ExpressionAstBuilder::BuildUnaryExpression(
    syntax_tree::NodePtr root
) {
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
