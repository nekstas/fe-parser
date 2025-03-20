#include "number_expression.h"

#include "../visitors/visitor.h"

void ast::NumberExpression::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

ast::NumberExpression::NumberExpression(const std::string& number) : number_(number) {
}
