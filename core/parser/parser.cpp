#include "parser.h"

Parser::Parser(const Grammar& grammar, const std::vector<Token>& tokens)
    : grammar_(grammar), tokens_(tokens) {
}

syntax_tree::NodePtr Parser::Parse() {
    grammar_rules::GrammarRule main_rule = grammar_.GetMainRule();
    if (!main_rule) {
        throw MainRuleDoesNotExistError{};
    }

    syntax_tree::NodePtr result = main_rule->Parse(tokens_, grammar_);
    // TODO: uncomment it
    // if (!tokens_.Eof()) {
    //     throw ExtraTokensInCodeError{};
    // }

    return result;
}
