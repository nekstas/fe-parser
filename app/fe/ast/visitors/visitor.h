#pragma once

#include "../expressions/binary_expression.h"
#include "../fwd.h"

namespace ast {

class Visitor {
public:
    virtual ~Visitor() {
    }

    virtual void Visit(const NumberExpression& number) = 0;
    virtual void Visit(const VariableExpression& variable) = 0;
    virtual void Visit(const CallExpression& call_expression) = 0;
    virtual void Visit(const BinaryExpression& binary_expression) = 0;
    virtual void Visit(const DefineVariableStatement& statement) = 0;
    virtual void Visit(const DefineFunctionStatement& statement) = 0;
    virtual void Visit(const DefineModuleStatement& statement) = 0;
    virtual void Visit(const CommonImportStatement& statement) = 0;
    virtual void Visit(const ImportAsStatement& statement) = 0;
    virtual void Visit(const ImportListStatement& statement) = 0;
    virtual void Visit(const Module& module) = 0;

private:
};

}  // namespace ast