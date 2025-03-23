#pragma once

#include <string>

#include "../expressions/call_expression.h"
#include "../expressions_info.h"
#include "../statements/import_as_statement.h"
#include "visitor.h"

namespace ast {

class FormatVisitorUnknownImportTypeError : public std::runtime_error {
public:
    FormatVisitorUnknownImportTypeError()
        : std::runtime_error{"FormatVisitor: Unknown type of import statement."} {
    }
};

class FormatVisitor : public Visitor {
private:
    static constexpr std::string kOneIndent = "  ";

public:
    FormatVisitor();

    void Visit(const NumberExpression& number) override;
    void Visit(const VariableExpression& variable) override;
    void Visit(const CallExpression& call_expression) override;
    void Visit(const BinaryExpression& binary_expression) override;
    void Visit(const DefineVariableStatement& statement) override;
    void Visit(const DefineFunctionStatement& statement) override;
    void Visit(const DefineModuleStatement& statement) override;
    void Visit(const CommonImportStatement& statement) override;
    void Visit(const ImportAsStatement& statement) override;
    void Visit(const ImportListStatement& statement) override;
    void Visit(const Module& module) override;
    std::string GetResult() const {
        return result_.str();
    }

private:
    void FormatBraces(std::shared_ptr<Expression> expression, bool condition);

    void ProcessPartOfBinaryExpression(
        std::shared_ptr<Expression> expression, const ast::ExpressionsInfo::Info& info,
        ExpressionsInfo::Associativity place
    );

    void FormatExpressionsList(const std::vector<std::shared_ptr<Expression>>& list);

    void FormatIdentifiersList(const std::vector<std::string>& list);

    void FormatWherePart(std::shared_ptr<Module> module);

private:
    std::stringstream result_;
    std::string indent_;
    bool is_first_atom_ = true;
    ExpressionsInfo expressions_info_;
};

}  // namespace ast
