#include "grammar.h"

void Grammar::AddRule(const std::string& name, grammar_rules::GrammarRule rule) {
    if (rules_.contains(name)) {
        throw RuleAlreadyExistsError{name};
    } else if (!rule) {
        throw EmptyRuleError{
            FormatStream() << "Attempt to add empty rule to grammar with name \"" << name << "\""
        };
    }

    rules_[name] = rule;
}

grammar_rules::GrammarRule Grammar::GetRule(const std::string& name) const {
    if (!rules_.contains(name)) {
        return {};
    }
    return rules_.at(name);
}

void Grammar::SetMainRule(const std::string& name) {
    main_rule_ = name;
}

grammar_rules::GrammarRule Grammar::GetMainRule() const {
    return GetRule(main_rule_);
}
