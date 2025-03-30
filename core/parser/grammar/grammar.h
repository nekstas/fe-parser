#pragma once

#include <map>

#include "../../../utils/format_stream.h"
#include "rules/abstract_rule.h"

struct RuleAlreadyExistsError : public std::runtime_error {
public:
    RuleAlreadyExistsError(const std::string& name)
        : std::runtime_error{
              FormatStream() << "Rule with name \"" << name << "\" already exists."
          } {
    }
};

struct EmptyRuleError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class Grammar {
public:
    Grammar() = default;

    void AddRule(const std::string& name, const grammar_rules::GrammarRule& rule);

    grammar_rules::GrammarRule GetRule(const std::string& name) const;

    void SetMainRule(const std::string& name);

    grammar_rules::GrammarRule GetMainRule() const;

private:
    std::map<std::string, grammar_rules::GrammarRule> rules_;
    std::string main_rule_;
};
