#pragma once
#include "../token_types.h"

struct BaseToken {
public:
    virtual ~BaseToken() {
    }

    virtual TokenType GetType() const = 0;
};
