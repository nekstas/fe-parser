#include "variable_expression.h"

#include "../visitors/visitor.h"

ast::VariableExpression::VariableExpression(const std::string& name) : name_(name) {
}

void ast::VariableExpression::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::VariableExpression::GetName() const {
    return name_;
}
