#pragma once

#include <string>
#include <vector>

#include "expression.h"

namespace ast {

class CallExpression : public Expression {
public:
    CallExpression(
        std::shared_ptr<Expression> callee, const std::vector<std::shared_ptr<Expression>>& args
    );

    void Accept(Visitor& visitor) override;

    std::shared_ptr<Expression> GetCallee() const;

    std::vector<std::shared_ptr<Expression>> GetArgs() const;

private:
    std::shared_ptr<Expression> callee_;
    std::vector<std::shared_ptr<Expression>> args_;
};

}  // namespace ast
