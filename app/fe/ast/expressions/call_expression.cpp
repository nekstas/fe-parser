#include "call_expression.h"

#include "../visitors/visitor.h"

ast::CallExpression::CallExpression(
    std::shared_ptr<Expression> callee, const std::vector<std::shared_ptr<Expression>>& args
)
    : callee_(callee), args_(args) {
}

void ast::CallExpression::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

std::shared_ptr<ast::Expression> ast::CallExpression::GetCallee() const {
    return callee_;
}

std::vector<std::shared_ptr<ast::Expression>> ast::CallExpression::GetArgs() const {
    return args_;
}
