#pragma once

#include <string>

#include "../expressions/call_expression.h"
#include "visitor.h"

namespace ast {

class FormatVisitor : public Visitor {
public:
    void Visit(const NumberExpression& number) override;
    void Visit(const VariableExpression& number) override;
    void Visit(const CallExpression& number) override;

    const std::string& GetResult() const {
        return result_;
    }

private:
    std::string result_;
};

}  // namespace ast
