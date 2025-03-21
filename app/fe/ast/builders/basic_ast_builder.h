#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "../../../../core/parser/grammar/syntax_tree/node.h"
#include "../../../../core/parser/grammar/syntax_tree/token_node.h"
#include "../expressions_info.h"
#include "../node.h"

namespace ast {

class AstBuilderError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class BasicAstBuilder {
protected:
    static constexpr std::string kExtendedIdentifierSeparator = ".";

public:
    BasicAstBuilder() = default;

protected:
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
};

}  // namespace ast
