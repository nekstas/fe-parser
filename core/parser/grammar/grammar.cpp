#include "grammar.h"

void Grammar::AddRule(const std::string& name, const GrammarRule& rule) {
    if (rules_.contains(name)) {
        throw RuleAlreadyExistsError{name};
    }

    rules_[name] = rule;
}

void Grammar::SetMainRule(const std::string& name) {
    main_rule_ = name;
}
