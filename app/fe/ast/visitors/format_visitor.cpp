#include "format_visitor.h"

#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"

void ast::FormatVisitor::Visit(const ast::NumberExpression& number) {
    result_ = number.GetValue();
}

void ast::FormatVisitor::Visit(const ast::VariableExpression& variable) {
    result_ = variable.GetName();
}
