#pragma once

#include <string>

#include "../expressions/call_expression.h"
#include "../expressions_info.h"
#include "../statements/import_as_statement.h"
#include "visitor.h"

namespace ast {

class FormatVisitor : public Visitor {
public:
    FormatVisitor();

    void Visit(const NumberExpression& number) override;
    void Visit(const VariableExpression& variable) override;
    void Visit(const CallExpression& call_expression) override;
    void Visit(const BinaryExpression& binary_expression) override;
    void Visit(const DefineVariableStatement& statement) override;
    void Visit(const DefineFunctionStatement& statement) override;
    void Visit(const CommonImportStatement& statement) override;
    void Visit(const ImportAsStatement& statement) override;
    void Visit(const ImportListStatement& statement) override;
    void Visit(const Module& module) override;

    const std::string& GetResult() const {
        return result_;
    }

private:
    void ProcessPartOfBinaryExpression(
        FormatStream& new_result, std::shared_ptr<Expression> expression,
        const ast::ExpressionsInfo::Info& info, ExpressionsInfo::Associativity place
    );

    std::string FormatExpressionsList(const std::vector<std::shared_ptr<Expression>>& list);

    std::string FormatIdentifiersList(const std::vector<std::string>& list);

private:
    std::string result_;
    ExpressionsInfo expressions_info_;
};

}  // namespace ast
