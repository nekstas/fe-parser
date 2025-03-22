#pragma once

#include <string>

#include "import_statement.h"

namespace ast {

class ImportAsStatement : public ImportStatement {
public:
    ImportAsStatement(const std::string& name, const std::string& alias);

    void Accept(Visitor& visitor) override;

    const std::string& GetAlias() const;

private:
    std::string alias_;
};

}  // namespace ast
