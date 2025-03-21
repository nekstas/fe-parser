#pragma once

#include <string>

#include "statement.h"

namespace ast {

class DefineVariableStatement : public Statement {
public:
    DefineVariableStatement(const std::string& name, std::shared_ptr<Expression> expression);

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

    std::shared_ptr<Expression> GetExpression() const;

private:
    std::string name_;
    std::shared_ptr<Expression> expression_;
};

}  // namespace ast
