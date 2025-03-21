#pragma once

#include <string>
#include <vector>

#include "import_statement.h"

namespace ast {

class CommonImportStatement : public ImportStatement {
public:
    CommonImportStatement(const std::string& name);

    void Accept(Visitor& visitor) override;

private:
};

}  // namespace ast
