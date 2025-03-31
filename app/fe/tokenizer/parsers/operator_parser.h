#pragma once

#include "../../../../core/algo/search_set.hpp"
#include "../../../../core/tokenizer/abstract_parser.h"
#include "../../../../utils/useful_streams.h"
#include "../../../../utils/utils.h"
#include "../tokens/operator_token.h"

class OperatorParser : public AbstractParser {
public:
    OperatorParser(const SearchSet& operators);

    virtual Token TryParse(CodeStream& stream) const override;

private:
    SearchSet operators_;
};
