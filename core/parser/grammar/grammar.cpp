#include "grammar.h"

void Grammar::AddRule(const std::string& name, const grammar_rules::GrammarRule& rule) {
    if (rules_.contains(name)) {
        throw RuleAlreadyExistsError{name};
    }

    rules_[name] = rule;
}

void Grammar::SetMainRule(const std::string& name) {
    main_rule_ = name;
}

grammar_rules::GrammarRule Grammar::GetMainRule() const {
    if (!rules_.contains(main_rule_)) {
        return {};
    }
    return rules_.at(main_rule_);
}
