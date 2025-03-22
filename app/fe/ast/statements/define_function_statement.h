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

    DefineFunctionStatement(
        const std::string& name, const std::vector<std::string>& args,
        std::shared_ptr<Expression> expression, std::shared_ptr<Module> where_module
    );

    void Accept(Visitor& visitor) override;

    const std::string& GetName() const;

    const std::vector<std::string>& GetArgs() const;

    std::shared_ptr<Expression> GetExpression() const;

    std::shared_ptr<Module> GetWhereModule() const;

private:
    std::string name_;
    std::vector<std::string> args_;
    std::shared_ptr<Expression> expression_;
    std::shared_ptr<Module> where_module_;
};

}  // namespace ast
