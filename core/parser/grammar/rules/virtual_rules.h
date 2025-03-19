#pragma once

#include "abstract_rule.h"

namespace grammar_rules {

class VirtualRules {
public:
    static GrammarRule SeparatorRule(
        GrammarRule element, GrammarRule separator, bool accept_empty = false
    );

    static GrammarRule KeywordRule(const std::string& name);

    static GrammarRule OperatorRule(const std::string& code);

    static GrammarRule Choice(std::initializer_list<std::string> names);

    static GrammarRule OpenBracket();

    static GrammarRule CloseBracket();

    static GrammarRule NewLine();
};

}  // namespace grammar_rules
