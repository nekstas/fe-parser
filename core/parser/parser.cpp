#include "parser.h"

Parser::Parser(const Grammar& grammar) : grammar_(grammar) {
}

syntax_tree::NodePtr Parser::Parse(const std::vector<Token>& tokens) {
    tokens_ = TokensStream{tokens};
    grammar_rules::GrammarRule main_rule = grammar_.GetMainRule();
    if (!main_rule) {
        throw MainRuleDoesNotExistError{};
    }

    syntax_tree::NodePtr result = main_rule->Parse(tokens_, grammar_);
    if (!tokens_.Eof()) {
        throw ExtraTokensInCodeError{};
    }

    return result;
}
