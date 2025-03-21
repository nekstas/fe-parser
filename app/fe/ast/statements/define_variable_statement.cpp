#include "define_variable_statement.h"

#include "../visitors/visitor.h"

ast::DefineVariableStatement::DefineVariableStatement(
    const std::string& name, std::shared_ptr<ast::Expression> expression
)
    : name_(name), expression_(expression) {
}

void ast::DefineVariableStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::DefineVariableStatement::GetName() const {
    return name_;
}

std::shared_ptr<ast::Expression> ast::DefineVariableStatement::GetExpression() const {
    return expression_;
}
