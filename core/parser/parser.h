#pragma once

#include "../../utils/useful_streams.h"
#include "../tokenizer/abstract_token.h"
#include "grammar/grammar.h"
#include "grammar/syntax_tree/node.h"

struct MainRuleDoesNotExistError : public std::runtime_error {
public:
    MainRuleDoesNotExistError() : std::runtime_error{"Main rule of given grammar does not exist."} {
    }
};

struct ExtraTokensInCodeError : public std::runtime_error {
public:
    ExtraTokensInCodeError() : std::runtime_error{"The are extra tokens after parsing code."} {
    }
};

class Parser {
public:
    Parser(const Grammar& grammar);

    syntax_tree::NodePtr Parse(const std::vector<Token>& tokens);

private:
    Grammar grammar_;
    TokensStream tokens_;
};