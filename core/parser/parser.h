#pragma once

#include "../../utils/useful_streams.h"
#include "../tokenizer/tokens/abstract_token.h"
#include "grammar/grammar.h"
#include "grammar/syntax_tree/node.h"

/*
 // должны быть правила для парсинга левосторонней рекурсии (циклическое может быть)
 RuleHandler* {} ()  // абстрактный класс обработчика результата применения правила
 // может быть это надо вынести в другое место, пока не очень понятно
 */

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
    Parser(const Grammar& grammar, const std::vector<Token>& tokens);

    syntax_tree::NodePtr Parse();

private:
    Grammar grammar_;
    TokensStream tokens_;
};