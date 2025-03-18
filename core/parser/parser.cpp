#include "parser.h"

Parser::Parser(const Grammar& grammar, const std::vector<Token>& tokens)
    : grammar_(grammar), tokens_(tokens) {
}

syntax_tree::NodePtr Parser::Parse() {
    grammar_rules::GrammarRule main_rule = grammar_.GetMainRule();
    if (!main_rule) {
        throw MainRuleDoesNotExistError{};
    }

    return main_rule->Parse(tokens_, grammar_);
}
