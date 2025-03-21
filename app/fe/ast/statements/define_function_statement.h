#pragma once

#include <string>
#include <vector>

#include "statement.h"

namespace ast {

class DefineFunctionStatement : public Statement {
public:
    DefineFunctionStatement(
        const std::string& name, const std::vector<std::string>& args,
        std::shared_ptr<Expression> expression
    );

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

    const std::vector<std::string>& GetArgs() const;

    std::shared_ptr<Expression> GetExpression() const;

private:
    std::string name_;
    std::vector<std::string> args_;
    std::shared_ptr<Expression> expression_;
};

}  // namespace ast
