#pragma once

#include <string>

#include "atom_expression.h"

namespace ast {

class NumberExpression : public AtomExpression {
public:
    NumberExpression(const std::string& number);

    void Accept(Visitor& visitor) override;

    const std::string& GetValue() const;

    bool IsNegative() const;

private:
    std::string number_;
};

}  // namespace ast
