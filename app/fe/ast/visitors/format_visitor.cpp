#include "format_visitor.h"

#include "../expressions/number_expression.h"

void ast::FormatVisitor::Visit(const ast::NumberExpression& number) {
    result_ = number.GetValue();
}
