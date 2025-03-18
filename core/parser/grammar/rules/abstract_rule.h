#pragma once

#include "../../../../utils/useful_streams.h"
#include "../syntax_tree/node.h"

class Grammar;

namespace grammar_rules {

class AbstractRule {
public:
    virtual ~AbstractRule() {
    }

    virtual syntax_tree::NodePtr Parse(TokensStream& tokens, const Grammar& grammar) = 0;

private:
};

using GrammarRule = std::shared_ptr<AbstractRule>;

template <typename T, typename... Args>
GrammarRule MakeRule(Args&&... args) {
    return std::make_shared<T>(args...);
};

}  // namespace grammar_rules
