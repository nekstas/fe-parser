#pragma once

#include <string>
#include <vector>

#include "statement.h"

namespace ast {

class DefineModuleStatement : public Statement {
public:
    DefineModuleStatement(const std::string& name, std::shared_ptr<Module> module);

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

    std::shared_ptr<Module> GetModule() const;

private:
    std::string name_;
    std::shared_ptr<Module> module_;
};

}  // namespace ast
