#pragma once

#include "../../../../core/algo/search_set.hpp"
#include "../../../../core/tokenizer/abstract_parser.h"
#include "../../../../utils/useful_streams.h"

class NameKeywordParser : public AbstractParser {
private:
    static constexpr char kUnderscoreChar = '_';

public:
    explicit NameKeywordParser(const SearchSet& keywords);

public:
    virtual Token TryParse(CodeStream& stream) const override;

private:
    bool IsStartSymbol(char c) const;

    bool IsMiddleSymbol(char c) const;

private:
    SearchSet keywords_;
};
