#pragma once

#include "../ast/expressions_info.h"

namespace fe {

class ExpressionsInfoFactory {
public:
    ExpressionsInfoFactory() = default;

    ast::ExpressionsInfo Create() const;
};

};  // namespace fe
