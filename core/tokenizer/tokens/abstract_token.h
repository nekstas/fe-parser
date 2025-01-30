#pragma once
#include "../token_types.h"

struct AbstractToken {
public:
    virtual ~AbstractToken() {
    }

    virtual TokenType GetType() const = 0;
};
