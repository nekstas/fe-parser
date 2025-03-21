#include "common_import_statement.h"

#include "../visitors/visitor.h"

ast::CommonImportStatement::CommonImportStatement(const std::string& name) : ImportStatement(name) {
}

void ast::CommonImportStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}
