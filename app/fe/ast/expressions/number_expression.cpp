#include "number_expression.h"

#include "../visitors/visitor.h"

void ast::NumberExpression::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

ast::NumberExpression::NumberExpression(const std::string& number) : number_(number) {
}

const std::string& ast::NumberExpression::GetValue() const {
    return number_;
}

bool ast::NumberExpression::IsNegative() const {
    return number_.front() == '-';
}
