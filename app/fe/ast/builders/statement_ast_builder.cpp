#include "statement_ast_builder.h"

#include "../statements/common_import_statement.h"
#include "../statements/define_function_statement.h"
#include "../statements/define_variable_statement.h"

std::shared_ptr<ast::Statement> ast::StatementAstBuilder::BuildStatement(syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "statement");
    auto [option, child] = UnpackVariantNode(node);

    // "import_statement", "define_variable_statement",
    // "define_function_statement", "define_module_statement"

    switch (option) {
        case 0:
            return BuildImportStatement(child);
        case 1:
            return BuildDefineVariableStatement(child);
        case 2:
            return BuildDefineFunctionStatement(child);
        case 3:  // define_module_statement
            break;
    }

    throw AstBuilderError{"Expected more case handlers of statement."};
}

std::shared_ptr<ast::DefineVariableStatement>
ast::StatementAstBuilder::BuildDefineVariableStatement(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "define_variable_statement");
    auto name = GetIdentifier(GetChild(node, 1));
    auto expression = BuildExpression(GetChild(node, 3));

    return MakeNode<DefineVariableStatement>(name, expression);
}

std::shared_ptr<ast::DefineFunctionStatement>
ast::StatementAstBuilder::BuildDefineFunctionStatement(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "define_function_statement");
    auto name = GetIdentifier(GetChild(node, 1));
    auto args = GetIdentifierArgsList(GetChild(node, 2));
    auto expression = BuildExpression(GetChild(node, 4));

    return MakeNode<DefineFunctionStatement>(name, args, expression);
}

std::shared_ptr<ast::ImportStatement> ast::StatementAstBuilder::BuildImportStatement(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "import_statement");
    auto name = GetExtendedIdentifier(GetChild(node, 1));

    auto additional_part = UnpackOptionalNode(GetChild(node, 2));

    if (!additional_part) {
        return MakeNode<CommonImportStatement>(name);
    }

    // TODO: check more cases here (as and (name_1, ..., name_n))
    return MakeNode<CommonImportStatement>("<not-working-now>");
}
