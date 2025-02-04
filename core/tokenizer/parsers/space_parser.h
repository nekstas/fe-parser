#pragma once

#include "../../../utils/utils.h"
#include "abstract_parser.h"

class SpaceParser : public AbstractParser {
public:
    bool IsActive(const TokenizerContext &context) const override;

    virtual std::optional<Token> TryParse(IStream& stream) const override;

    void ChangeContext(TokenizerContext &context) const override;

private:
    bool IsSpace(int32_t c) const;
};
