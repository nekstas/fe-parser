#include "import_list_statement.h"

#include "../visitors/visitor.h"

ast::ImportListStatement::ImportListStatement(
    const std::string& name, const std::vector<std::string>& list
)
    : ImportStatement(name), list_(list) {
}

void ast::ImportListStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::vector<std::string>& ast::ImportListStatement::GetList() const {
    return list_;
}
