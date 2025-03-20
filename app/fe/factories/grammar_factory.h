#pragma once

#include "../../../core/parser/grammar/grammar.h"

namespace fe {

class GrammarFactory {
public:
    GrammarFactory() = default;

    Grammar Create() const;
};

};  // namespace fe
