#pragma once

#include <string>
#include <vector>

#include "import_statement.h"

namespace ast {

class ImportListStatement : public ImportStatement {
public:
    ImportListStatement(const std::string& name, const std::vector<std::string>& list);

    void Accept(Visitor& visitor) override;

    const std::vector<std::string>& GetList() const;

private:
    std::vector<std::string> list_;
};

}  // namespace ast
