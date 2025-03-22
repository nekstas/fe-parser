#include "format_visitor.h"

#include "../../../../utils/utils.h"
#include "../../factories/expressions_info_factory.h"
#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"
#include "../module.h"
#include "../statements/common_import_statement.h"
#include "../statements/define_function_statement.h"
#include "../statements/define_module_statement.h"
#include "../statements/define_variable_statement.h"
#include "../statements/import_as_statement.h"
#include "../statements/import_list_statement.h"

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
    result_ << indent_ << "import " << name;
}

void ast::FormatVisitor::Visit(const ast::ImportAsStatement& statement) {
    auto name = statement.GetName();
    auto alias = statement.GetAlias();
    result_ << indent_ << "import " << name << " as " << alias;
}

void ast::FormatVisitor::Visit(const ast::ImportListStatement& statement) {
    auto name = statement.GetName();
    auto list = statement.GetList();

    result_ << indent_ << "import " << name;
    FormatIdentifiersList(list);
}

void ast::FormatVisitor::Visit(const ast::Module& module) {
    auto statements = module.GetStatements();

    for (auto statement : statements) {
        statement->Accept(*this);
        result_ << "\n";
    }
}

// TODO: handle -number case (braces or not?)
