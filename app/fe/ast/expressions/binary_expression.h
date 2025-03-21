#pragma once

#include <string>

#include "expression.h"

namespace ast {

class BinaryExpression : public Expression {
public:
    BinaryExpression(
        const std::string& code, std::shared_ptr<Expression> first,
        std::shared_ptr<Expression> second
    );

    void Accept(Visitor& visitor) override;

    const std::string& GetCode() const;

    std::shared_ptr<Expression> GetFirst() const;

    std::shared_ptr<Expression> GetSecond() const;

private:
    std::string code_;
    std::shared_ptr<Expression> first_;
    std::shared_ptr<Expression> second_;
};

}  // namespace ast
