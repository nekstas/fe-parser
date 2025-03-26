#pragma once

#include "../../../utils/useful_streams.h"
#include "../../algo/search_set.hpp"
#include "abstract_parser.h"

class NameKeywordParser : public AbstractParser {
private:
    static constexpr char kUnderscoreChar = '_';

public:
    NameKeywordParser(const SearchSet& keywords);

public:
    virtual Token TryParse(CodeStream& stream) const override;

private:
    bool IsStartSymbol(char c) const;

    bool IsMiddleSymbol(char c) const;

private:
    SearchSet keywords_;
};
