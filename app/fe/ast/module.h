#pragma once

#include <vector>

#include "node.h"
#include "statements/statement.h"

namespace ast {

class Module : public Node {
public:
    Module(const std::vector<std::shared_ptr<Statement>>& statements);

    void Accept(Visitor& visitor) override;

    const std::vector<std::shared_ptr<Statement>>& GetStatements() const;

private:
    std::vector<std::shared_ptr<Statement>> statements_;
};

}  // namespace ast
