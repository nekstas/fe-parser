#include "statement_ast_builder.h"

#include "../statements/define_variable_statement.h"

std::shared_ptr<ast::Statement> ast::StatementAstBuilder::BuildStatement(syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "statement");
    auto [option, child] = UnpackVariantNode(node);

    // "import_statement", "define_variable_statement",
    // "define_function_statement", "define_module_statement"

    switch (option) {
        case 0:  // import_statement
            break;
        case 1:
            return BuildDefineVariableStatement(child);
        case 2:  // define_function_statement
            break;
        case 3:  // define_module_statement
            break;
    }

    throw AstBuilderError{"Expected more case handlers of statement."};
}
std::shared_ptr<ast::Statement> ast::StatementAstBuilder::BuildDefineVariableStatement(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "define_variable_statement");
    auto name = GetIdentifier(GetChild(node, 1));
    auto expression = BuildExpression(GetChild(node, 3));

    return MakeNode<DefineVariableStatement>(name, expression);
}
