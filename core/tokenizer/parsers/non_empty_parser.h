#pragma once

#include "abstract_parser.h"

class NonEmptyParser : public AbstractParser {
public:
    virtual bool IsActive(const TokenizerContext& context) const override;

    virtual void ChangeContext(TokenizerContext& context) const override;
};

using TokenizerParser = std::unique_ptr<AbstractParser>;
