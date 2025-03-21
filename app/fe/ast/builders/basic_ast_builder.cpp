#include "basic_ast_builder.h"

#include "../../../../core/parser/grammar/syntax_tree/named_node.h"
#include "../../../../core/parser/grammar/syntax_tree/optional_node.h"
#include "../../../../core/parser/grammar/syntax_tree/repeat_node.h"
#include "../../../../core/parser/grammar/syntax_tree/variant_node.h"
#include "../../../../core/tokenizer/tokens/name_token.h"
#include "../../../../core/tokenizer/tokens/operator_token.h"
#include "../../../../utils/utils.h"
#include "../expressions/variable_expression.h"

void ast::BasicAstBuilder::Require(bool condition, std::string message) {
    if (!condition) {
        throw AstBuilderError{message};
    }
}

syntax_tree::NodePtr ast::BasicAstBuilder::GetChild(syntax_tree::NodePtr root, size_t index) {
    Require(static_cast<bool>(root), "GetChild: root can't be null pointer.");
    Require(index < root->GetChildren().size(), "GetChild: there are no child with given index.");

    return root->GetChildren()[index];
}

syntax_tree::NodePtr ast::BasicAstBuilder::UnpackNamedNode(
    syntax_tree::NodePtr root, const std::string& name
) {
    auto node = syntax_tree::Cast<syntax_tree::NamedNode>(root);
    Require(node->GetName() == name, "UnpackNamedNode: root name and expected name are not equal.");

    return node->GetChildren()[0];
}

std::pair<size_t, syntax_tree::NodePtr> ast::BasicAstBuilder::UnpackVariantNode(
    syntax_tree::NodePtr root
) {
    auto node = syntax_tree::Cast<syntax_tree::VariantNode>(root);

    return {node->GetOption(), node->GetChildren()[0]};
}

std::pair<size_t, std::vector<syntax_tree::NodePtr>> ast::BasicAstBuilder::UnpackRepeatNode(
    syntax_tree::NodePtr root
) {
    auto node = syntax_tree::Cast<syntax_tree::RepeatNode>(root);

    return {node->GetNodesCount(), node->GetChildren()};
}

syntax_tree::NodePtr ast::BasicAstBuilder::UnpackOptionalNode(syntax_tree::NodePtr root) {
    auto node = syntax_tree::Cast<syntax_tree::OptionalNode>(root);

    if (node->HasNode()) {
        return node->GetResult();
    }
    return {};
}

std::string ast::BasicAstBuilder::GetIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "identifier");
    auto token = ExtractToken<NameToken>(node);
    return token.GetName();
}

std::string ast::BasicAstBuilder::GetExtendedIdentifier(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "extended_identifier");

    std::vector<std::string> identifiers = {GetIdentifier(GetChild(node, 0))};
    auto [count, sequence] = UnpackRepeatNode(GetChild(node, 1));

    for (size_t i = 0; i < count; ++i) {
        auto child = sequence[i];
        identifiers.push_back(GetIdentifier(GetChild(child, 1)));
    }

    return utils::Join(identifiers, kExtendedIdentifierSeparator);
}

std::string ast::BasicAstBuilder::GetUnaryOperator(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "unary_operator");
    auto [_, token_node] = UnpackVariantNode(node);
    auto token = ExtractToken<OperatorToken>(token_node);
    return token.GetCode();
}
