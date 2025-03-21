#pragma once

#include <string>

#include "atom_expression.h"

namespace ast {

class NumberExpression : public AtomExpression {
public:
    NumberExpression(const std::string& number);

    void Accept(Visitor& visitor) override;

    const std::string& GetValue() const {
        return number_;
    }

private:
    std::string number_;
};

}  // namespace ast
