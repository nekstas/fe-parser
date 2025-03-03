#pragma once

#include "../../../utils/useful_streams.h"
#include "../../../utils/utils.h"
#include "../../algo/search_set.hpp"
#include "../tokens/operator_token.h"
#include "abstract_parser.h"

class OperatorParser : public AbstractParser {
public:
    OperatorParser(const SearchSet& operators);

    virtual Token TryParse(CodeStream& stream) const override;

private:
    SearchSet operators_;
};
