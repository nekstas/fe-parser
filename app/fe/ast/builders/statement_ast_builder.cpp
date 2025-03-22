#include "statement_ast_builder.h"

#include "../statements/common_import_statement.h"
#include "../statements/define_function_statement.h"
#include "../statements/define_variable_statement.h"
#include "../statements/import_as_statement.h"
#include "../statements/import_list_statement.h"

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

    auto [option, child] = UnpackVariantNode(additional_part);

    switch (option) {
        case 0:
            return BuildImportAsStatement(name, child);
        case 1:
            return BuildImportListStatement(name, child);
    }

    throw AstBuilderError{"Expected more case handlers of import statement."};
}

std::shared_ptr<ast::ImportStatement> ast::StatementAstBuilder::BuildImportAsStatement(
    const std::string& name, syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "as_part");
    auto alias = GetIdentifier(GetChild(node, 1));
    return MakeNode<ImportAsStatement>(name, alias);
}

std::shared_ptr<ast::ImportStatement> ast::StatementAstBuilder::BuildImportListStatement(
    const std::string& name, syntax_tree::NodePtr root
) {
    auto list = GetIdentifierArgsList(root);

    Require(!list.empty(), "Can't import empty list of identifiers from another module.");

    return MakeNode<ImportListStatement>(name, list);
}
