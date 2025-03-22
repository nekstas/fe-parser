#include "module.h"

#include "visitors/visitor.h"

ast::Module::Module(const std::vector<std::shared_ptr<Statement>>& statements)
    : statements_(statements) {
}

void ast::Module::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::vector<std::shared_ptr<ast::Statement>>& ast::Module::GetStatements() const {
    return statements_;
}
