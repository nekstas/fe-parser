#pragma once

#include <map>

#include "../../../utils/format_stream.h"
#include "rules/abstract_rule.h"

struct RuleAlreadyExistsError : public std::runtime_error {
public:
    RuleAlreadyExistsError(const std::string& name)
        : std::runtime_error{FormatStream()
                             << "Rule with name \"" << name << "\" already exists."} {
    }
};

class Grammar {
public:
    void AddRule(const std::string& name, const GrammarRule& rule);

    void SetMainRule(const std::string& name);

private:
    std::map<std::string, GrammarRule> rules_;
    std::string main_rule_;
};
