#pragma once
#include "../../../utils/istream.h"
#include "../tokenizer_context.h"
#include "../tokens/abstract_token.h"

class AbstractParser {
public:
    virtual ~AbstractParser() {
    }

    virtual bool IsActive(const TokenizerContext& context) const = 0;

    virtual std::optional<Token> TryParse(IStream& stream) const = 0;

    virtual void ChangeContext(TokenizerContext& context) const = 0;
};

using TokenizerParser = std::unique_ptr<AbstractParser>;
