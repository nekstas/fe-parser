#pragma once

#include <string>

#include "../expressions/call_expression.h"
#include "../expressions_info.h"
#include "visitor.h"

namespace ast {

class FormatVisitor : public Visitor {
public:
    FormatVisitor();

    void Visit(const NumberExpression& number) override;
    void Visit(const VariableExpression& variable) override;
    void Visit(const CallExpression& call_expression) override;
    void Visit(const BinaryExpression& binary_expression) override;

    const std::string& GetResult() const {
        return result_;
    }

private:
    void ProcessPartOfBinaryExpression(
        FormatStream& new_result, std::shared_ptr<Expression> expression,
        const ast::ExpressionsInfo::Info& info, ExpressionsInfo::Associativity place
    );

private:
    std::string result_;
    ExpressionsInfo expressions_info_;
};

}  // namespace ast
