#include "binary_expression.h"

#include "../visitors/visitor.h"

ast::BinaryExpression::BinaryExpression(
    const std::string& code, std::shared_ptr<Expression> first, std::shared_ptr<Expression> second
)
    : code_(code), first_(first), second_(second) {
}

void ast::BinaryExpression::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::BinaryExpression::GetCode() const {
    return code_;
}

std::shared_ptr<ast::Expression> ast::BinaryExpression::GetFirst() const {
    return first_;
}

std::shared_ptr<ast::Expression> ast::BinaryExpression::GetSecond() const {
    return second_;
}
