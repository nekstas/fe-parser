#include "named_rule.h"

#include "../grammar.h"
#include "../syntax_tree/named_node.h"

grammar_rules::NamedRule::NamedRule(const std::string& name) : name_(name) {
}

syntax_tree::NodePtr grammar_rules::NamedRule::Parse(TokensStream& tokens, const Grammar& grammar) {
    GrammarRule another_rule = grammar.GetRule(name_);
    if (!another_rule) {
        throw EmptyRuleError{FormatStream() << "Rule with name \"" << name_ << "\" not found."};
    }
    return MakeNode<syntax_tree::NamedNode>(name_, another_rule->Parse(tokens, grammar));
}
