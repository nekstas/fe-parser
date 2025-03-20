#pragma once

#include <string>

#include "visitor.h"

namespace ast {

class FormatVisitor : public Visitor {
public:
    void Visit(const NumberExpression& number) override;

    const std::string& GetResult() const {
        return result_;
    }

private:
    std::string result_;
};

}  // namespace ast
