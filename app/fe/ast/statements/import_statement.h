#pragma once

#include <string>

#include "../statements/statement.h"

namespace ast {

class ImportStatement : public Statement {
public:
    ImportStatement(const std::string& name);

    const std::string& GetName() const;

protected:
    std::string name_;
};

}  // namespace ast
