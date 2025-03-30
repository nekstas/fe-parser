#include "statement_ast_builder.h"

#include "../module.h"
#include "../statements/common_import_statement.h"
#include "../statements/define_function_statement.h"
#include "../statements/define_module_statement.h"
#include "../statements/define_variable_statement.h"
#include "../statements/import_as_statement.h"
#include "../statements/import_list_statement.h"

std::shared_ptr<ast::Module> ast::StatementAstBuilder::GetModuleFromWherePart(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "where_part");
    return BuildModule(GetChild(node, 2));
}

std::shared_ptr<ast::Module> ast::StatementAstBuilder::BuildModule(syntax_tree::NodePtr root) {
    auto node = UnpackNamedNode(root, "module");
    auto [count, sequence] = UnpackRepeatNode(node);

    std::vector<std::shared_ptr<Statement>> statements;

    for (size_t i = 0; i < count; ++i) {
        auto statement = BuildStatement(GetChild(sequence[i], 0));
        statements.push_back(statement);
    }

    return MakeNode<ast::Module>(statements);
}

std::shared_ptr<ast::Statement> ast::StatementAstBuilder::BuildStatement(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "statement");
    auto [option, child] = UnpackVariantNode(node);

    switch (option) {
        case 0:
            return BuildImportStatement(child);
        case 1:
            return BuildDefineVariableStatement(child);
        case 2:
            return BuildDefineFunctionStatement(child);
        case 3:
            return BuildDefineModuleStatement(child);
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

    auto where_part = UnpackOptionalNode(GetChild(node, 5));

    if (!where_part) {
        return MakeNode<DefineFunctionStatement>(name, args, expression);
    }

    auto module = GetModuleFromWherePart(where_part);
    return MakeNode<DefineFunctionStatement>(name, args, expression, module);
}

std::shared_ptr<ast::DefineModuleStatement> ast::StatementAstBuilder::BuildDefineModuleStatement(
    syntax_tree::NodePtr root
) {
    auto node = UnpackNamedNode(root, "define_module_statement");
    auto name = GetIdentifier(GetChild(node, 1));
    auto module = GetModuleFromWherePart(GetChild(node, 2));

    return MakeNode<DefineModuleStatement>(name, module);
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
