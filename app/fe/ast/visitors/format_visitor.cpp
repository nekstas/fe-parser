#include "format_visitor.h"

#include <algorithm>
#include <map>
#include <set>

#include "../../../../utils/utils.h"
#include "../../factories/expressions_info_factory.h"
#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"
#include "../module.h"
#include "../statements/common_import_statement.h"
#include "../statements/define_function_statement.h"
#include "../statements/define_module_statement.h"
#include "../statements/define_variable_statement.h"
#include "../statements/import_list_statement.h"

namespace {

struct ModuleInfo {
    std::vector<std::shared_ptr<ast::ImportStatement>> imports;
    std::vector<std::shared_ptr<ast::Statement>> other_statements;
    bool has_modules = false;
};

enum class ImportType { COMMON_IMPORT, IMPORT_AS, IMPORT_LIST };

ImportType GetImportType(std::shared_ptr<ast::ImportStatement> statement) {
    if (ast::Is<ast::CommonImportStatement>(statement)) {
        return ImportType::COMMON_IMPORT;
    } else if (ast::Is<ast::ImportAsStatement>(statement)) {
        return ImportType::IMPORT_AS;
    } else if (ast::Is<ast::ImportListStatement>(statement)) {
        return ImportType::IMPORT_LIST;
    }
    throw ast::FormatVisitorUnknownImportTypeError{};
}

bool CompareImports(
    std::shared_ptr<ast::ImportStatement> lhs, std::shared_ptr<ast::ImportStatement> rhs
) {
    auto lhs_type = GetImportType(lhs);
    auto rhs_type = GetImportType(rhs);

    if (lhs_type != rhs_type) {
        return lhs_type < rhs_type;
    }

    if (lhs->GetName() != rhs->GetName()) {
        return lhs->GetName() < rhs->GetName();
    }

    if (lhs_type == ImportType::IMPORT_AS) {
        auto lhs_as = ast::Cast<ast::ImportAsStatement>(lhs);
        auto rhs_as = ast::Cast<ast::ImportAsStatement>(rhs);

        return lhs_as->GetAlias() < rhs_as->GetAlias();
    }

    return false;
}

ModuleInfo GetModuleInfo(const ast::Module& module) {
    ModuleInfo module_info;

    std::map<std::string, std::set<std::string>> import_data;

    for (auto statement : module.GetStatements()) {
        if (ast::Is<ast::ImportStatement>(statement)) {
            if (!ast::Is<ast::ImportListStatement>(statement)) {
                module_info.imports.push_back(ast::Cast<ast::ImportStatement>(statement));
            } else {
                auto list_import = ast::Cast<ast::ImportListStatement>(statement);
                auto module_name = list_import->GetName();
                auto list = list_import->GetList();
                auto& set = import_data[module_name];
                set.insert(list.begin(), list.end());
            }
        } else {
            module_info.other_statements.push_back(statement);
            module_info.has_modules |= ast::Is<ast::DefineModuleStatement>(statement);
        }
    }

    for (const auto& [name, set_of_identifiers] : import_data) {
        std::vector<std::string> list_of_identifiers(
            set_of_identifiers.begin(), set_of_identifiers.end()
        );
        module_info.imports.push_back(
            ast::MakeNode<ast::ImportListStatement>(name, list_of_identifiers)
        );
    }

    std::stable_sort(module_info.imports.begin(), module_info.imports.end(), CompareImports);

    return module_info;
}

}  // namespace

ast::FormatVisitor::FormatVisitor() : expressions_info_(fe::ExpressionsInfoFactory().Create()) {
}

void ast::FormatVisitor::FormatBraces(std::shared_ptr<Expression> expression, bool condition) {
    if (condition) {
        result_ << "(";
        expression->Accept(*this);
        result_ << ")";
    } else {
        expression->Accept(*this);
    }
}

void ast::FormatVisitor::FormatExpressionsList(const std::vector<std::shared_ptr<Expression>>& list
) {
    result_ << "(";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i != 0) {
            result_ << ", ";
        }
        list[i]->Accept(*this);
    }
    result_ << ")";
}

void ast::FormatVisitor::FormatIdentifiersList(const std::vector<std::string>& list) {
    result_ << "(" + utils::Join(list, ", ") + ")";
}

void ast::FormatVisitor::FormatWherePart(std::shared_ptr<Module> module) {
    result_ << " where\n";
    indent_ += kOneIndent;
    module->Accept(*this);
    indent_.erase(indent_.end() - kOneIndent.size(), indent_.end());
}

void ast::FormatVisitor::Visit(const ast::NumberExpression& number) {
    result_ << number.GetValue();
}

void ast::FormatVisitor::Visit(const ast::VariableExpression& variable) {
    result_ << variable.GetName();
}

void ast::FormatVisitor::Visit(const ast::CallExpression& call_expression) {
    auto callee = call_expression.GetCallee();

    FormatBraces(callee, !Is<VariableExpression>(callee));
    FormatExpressionsList(call_expression.GetArgs());
}

void ast::FormatVisitor::ProcessPartOfBinaryExpression(
    std::shared_ptr<Expression> expression, const ast::ExpressionsInfo::Info& info,
    ExpressionsInfo::Associativity place
) {
    bool without_braces = Is<AtomExpression>(expression);

    if (Is<BinaryExpression>(expression)) {
        auto binary_expr = Cast<BinaryExpression>(expression);
        auto another_info = expressions_info_.GetInfo(binary_expr->GetCode());

        without_braces = another_info.priority > info.priority ||
                         (another_info.priority == info.priority &&
                          ast::ExpressionsInfo::IsCompatible(info.associativity, place));
    }

    FormatBraces(expression, !without_braces);
}

void ast::FormatVisitor::Visit(const ast::BinaryExpression& binary_expression) {
    auto code = binary_expression.GetCode();
    auto first = binary_expression.GetFirst();
    auto second = binary_expression.GetSecond();
    auto info = expressions_info_.GetInfo(code);

    ProcessPartOfBinaryExpression(first, info, ExpressionsInfo::Associativity::LEFT);
    result_ << " " << binary_expression.GetCode() << " ";
    ProcessPartOfBinaryExpression(second, info, ExpressionsInfo::Associativity::RIGHT);
}

void ast::FormatVisitor::Visit(const ast::DefineVariableStatement& statement) {
    auto name = statement.GetName();
    auto expression = statement.GetExpression();

    result_ << indent_ << "let " << name << " := ";
    expression->Accept(*this);
    result_ << "\n";
}

void ast::FormatVisitor::Visit(const ast::DefineFunctionStatement& statement) {
    auto name = statement.GetName();
    auto args = statement.GetArgs();
    auto expression = statement.GetExpression();
    auto where_module = statement.GetWhereModule();

    result_ << indent_ << "let " << name;
    FormatIdentifiersList(args);
    result_ << " := ";
    expression->Accept(*this);

    if (where_module) {
        FormatWherePart(where_module);
    } else {
        result_ << "\n";
    }
}

void ast::FormatVisitor::Visit(const ast::DefineModuleStatement& statement) {
    auto name = statement.GetName();
    auto module = statement.GetModule();
    result_ << "module " << name;
    FormatWherePart(module);
}

void ast::FormatVisitor::Visit(const ast::CommonImportStatement& statement) {
    auto name = statement.GetName();
    result_ << indent_ << "import " << name << "\n";
}

void ast::FormatVisitor::Visit(const ast::ImportAsStatement& statement) {
    auto name = statement.GetName();
    auto alias = statement.GetAlias();
    result_ << indent_ << "import " << name << " as " << alias << "\n";
}

void ast::FormatVisitor::Visit(const ast::ImportListStatement& statement) {
    auto name = statement.GetName();
    auto list = statement.GetList();

    result_ << indent_ << "import " << name << " ";
    FormatIdentifiersList(list);
    result_ << "\n";
}

void ast::FormatVisitor::Visit(const ast::Module& module) {
    auto module_info = GetModuleInfo(module);
    size_t imports_count = module_info.imports.size();
    size_t statements_count = module_info.other_statements.size();

    for (auto statement : module_info.imports) {
        statement->Accept(*this);
    }

    if (module_info.has_modules && imports_count > 0 && statements_count > 0) {
        result_ << "\n";
    }

    for (size_t i = 0; i < statements_count; ++i) {
        module_info.other_statements[i]->Accept(*this);
        if (module_info.has_modules && i != statements_count - 1) {
            result_ << "\n";
        }
    }
}

// TODO: handle -number case (braces or not?)
