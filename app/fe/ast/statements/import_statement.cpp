#include "import_statement.h"

ast::ImportStatement::ImportStatement(const std::string& name) : name_(name) {
}

const std::string& ast::ImportStatement::GetName() const {
    return name_;
}
