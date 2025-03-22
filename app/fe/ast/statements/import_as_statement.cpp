#include "import_as_statement.h"

#include "../visitors/visitor.h"

ast::ImportAsStatement::ImportAsStatement(const std::string& name, const std::string& alias)
    : ImportStatement(name), alias_(alias) {
}

void ast::ImportAsStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::ImportAsStatement::GetAlias() const {
    return alias_;
}
