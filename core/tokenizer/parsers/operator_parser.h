#pragma once

#include "../../../utils/utils.h"
#include "../../algo/search_set.hpp"
#include "../../common/code/code_stream.h"
#include "../tokens/operator_token.h"
#include "abstract_parser.h"

class OperatorParser : public AbstractParser {
public:
    OperatorParser(const SearchSet& operators);

    virtual Token TryParse(CodeStream &stream) const override;

private:
    SearchSet operators_;
};
