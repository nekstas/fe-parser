#pragma once

#include "../fwd.h"

namespace ast {

class Visitor {
public:
    virtual ~Visitor() {
    }

    virtual void Visit(const NumberExpression& number) = 0;

private:

};

}  // namespace ast