#pragma once

#include <string>

#include "atom_expression.h"

namespace ast {

class VariableExpression : public AtomExpression {
public:
    VariableExpression(const std::string& name);

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

private:
    std::string name_;
};

}  // namespace ast
