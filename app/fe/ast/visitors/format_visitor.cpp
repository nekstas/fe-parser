#include "format_visitor.h"

#include "../../../../utils/format_stream.h"
#include "../../../../utils/utils.h"
#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"

void ast::FormatVisitor::Visit(const ast::NumberExpression& number) {
    result_ = number.GetValue();
}

void ast::FormatVisitor::Visit(const ast::VariableExpression& variable) {
    result_ = variable.GetName();
}

void ast::FormatVisitor::Visit(const ast::CallExpression& call_expression) {
    auto callee = call_expression.GetCallee();
    callee->Accept(*this);

    FormatStream new_result;

    if (!Is<VariableExpression>(callee)) {
        result_ = "(" + result_ + ")";
    }
    new_result << result_;

    std::vector<std::string> args;
    for (auto arg : call_expression.GetArgs()) {
        arg->Accept(*this);
        args.push_back(result_);
    }

    new_result << "(" << utils::Join(args, ", ") << ")";
    result_ = new_result;
}
