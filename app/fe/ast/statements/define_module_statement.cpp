#include "define_module_statement.h"

#include "../visitors/visitor.h"

ast::DefineModuleStatement::DefineModuleStatement(
    const std::string& name, std::shared_ptr<Module> module
)
    : name_(name), module_(module) {
}

void ast::DefineModuleStatement::Accept(ast::Visitor& visitor) {
    visitor.Visit(*this);
}

const std::string& ast::DefineModuleStatement::GetName() const {
    return name_;
}

std::shared_ptr<ast::Module> ast::DefineModuleStatement::GetModule() const {
    return module_;
}
