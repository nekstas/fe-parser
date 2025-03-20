#pragma once

#include <string>

#include "expression.h"

namespace ast {

class VariableExpression : public Expression {
public:
    VariableExpression(const std::string& name);

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

private:
    std::string name_;
};

}  // namespace ast
