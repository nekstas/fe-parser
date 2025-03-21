#include "define_function_statement.h"

#include "../visitors/visitor.h"

ast::DefineFunctionStatement::DefineFunctionStatement(
    const std::string& name, const std::vector<std::string>& args,
    std::shared_ptr<Expression> expression
)
    : name_(name), args_(args), expression_(expression) {
}

void ast::DefineFunctionStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::DefineFunctionStatement::GetName() const {
    return name_;
}

const std::vector<std::string>& ast::DefineFunctionStatement::GetArgs() const {
    return args_;
}

std::shared_ptr<ast::Expression> ast::DefineFunctionStatement::GetExpression() const {
    return expression_;
}
