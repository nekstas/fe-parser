#pragma once

#include "../../../core/parser/parser.h"

namespace fe {

class ParserFactory {
public:
    ParserFactory() = default;

    Parser Create() const;
};

};  // namespace fe
