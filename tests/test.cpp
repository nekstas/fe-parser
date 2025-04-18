#include <catch2/catch_test_macros.hpp>

#include "../../../core/parser/grammar/rules/named_rule.h"
#include "../../../core/parser/grammar/rules/optional_rule.h"
#include "../../../core/parser/grammar/rules/repeat_rule.h"
#include "../../../core/parser/grammar/rules/sequence_rule.h"
#include "../../../core/parser/grammar/rules/token_type_rule.hpp"
#include "../../../core/parser/grammar/rules/token_value_rule.hpp"
#include "../../../core/parser/grammar/rules/variant_rule.h"
#include "../../../core/parser/grammar/rules/virtual_rules.h"
#include "../../../core/parser/grammar/syntax_tree/named_node.h"
#include "../../../core/parser/grammar/syntax_tree/optional_node.h"
#include "../../../core/parser/grammar/syntax_tree/repeat_node.h"
#include "../../../core/parser/grammar/syntax_tree/sequence_node.h"
#include "../../../core/parser/grammar/syntax_tree/variant_node.h"
#include "../app/fe/ast/builders/full_ast_builder.h"
#include "../app/fe/ast/visitors/format_visitor.h"
#include "../app/fe/factories/parser_factory.h"
#include "../app/fe/factories/tokenizer_factory.h"
#include "../app/fe/tokenizer/parsers/integer_parser.h"
#include "../app/fe/tokenizer/parsers/name_keyword_parser.h"
#include "../app/fe/tokenizer/parsers/operator_parser.h"
#include "../app/fe/tokenizer/parsers/whitespace_parser.h"
#include "../app/fe/tokenizer/tokens/close_bracket_token.h"
#include "../app/fe/tokenizer/tokens/indent_token.h"
#include "../app/fe/tokenizer/tokens/integer_token.h"
#include "../app/fe/tokenizer/tokens/keyword_token.h"
#include "../app/fe/tokenizer/tokens/name_token.h"
#include "../app/fe/tokenizer/tokens/new_line_token.h"
#include "../app/fe/tokenizer/tokens/open_bracket_token.h"
#include "../app/fe/tokens_preprocessor/tokens_preprocessor.h"
#include "../core/parser/grammar/grammar.h"
#include "../core/parser/parser.h"

template <typename TokenType>
bool CheckEquals(Token token, const TokenType& expected) {
    return IsToken<TokenType>(token) && *ConvertTokenTo<TokenType>(token) == expected;
}

syntax_tree::NodePtr BuildSyntaxTree(
    const std::string& code, grammar_rules::GrammarRule rule, const Grammar& grammar
) {
    lex::Tokenizer tokenizer = fe::TokenizerFactory().Create();
    TokensPreprocessor tokens_preprocessor;
    auto tokens = tokenizer.Tokenize(code);
    TokensStream new_tokens(tokens_preprocessor.Process(tokens));
    return rule->Parse(new_tokens, grammar);
}

template <typename NodeType>
bool CheckNodeType(
    const std::string& code, grammar_rules::GrammarRule rule, const Grammar& grammar = {}
) {
    return syntax_tree::Is<NodeType>(BuildSyntaxTree(code, rule, grammar));
}

template <typename NodeType>
std::shared_ptr<NodeType> RequireNode(
    const std::string& code, grammar_rules::GrammarRule rule, const Grammar& grammar = {}
) {
    return syntax_tree::Cast<NodeType>(BuildSyntaxTree(code, rule, grammar));
}

bool CheckEmptyNode(
    const std::string& code, grammar_rules::GrammarRule rule, const Grammar& grammar = {}
) {
    return !BuildSyntaxTree(code, rule, grammar);
}

template <typename ParserType>
class ParserChecker {
public:
    ParserChecker(ParserType parser) : parser_(parser) {
    }

    bool CheckNull(const std::string& code) {
        CodeStream stream{code};
        return !parser_.TryParse(stream);
    }

    template <typename TokenType>
    bool CheckToken(const std::string& code, TokenType expected_token, size_t expected_pos) {
        CodeStream stream{code};
        Token token = parser_.TryParse(stream);

        return CheckEquals(token, expected_token) && stream.GetPos() == expected_pos;
    }

private:
    ParserType parser_;
};

TEST_CASE("Tokenizer Parsers") {
    SECTION("Integer Parser") {
        ParserChecker checker(IntegerParser{});
        CHECK(checker.CheckToken("0", IntegerToken{"0"}, 1));
        CHECK(checker.CheckToken("1", IntegerToken{"1"}, 1));
        CHECK(checker.CheckToken("12345", IntegerToken{"12345"}, 5));
        CHECK(checker.CheckToken("0111", IntegerToken{"0111"}, 4));
        CHECK(checker.CheckToken("0.", IntegerToken{"0"}, 1));
        CHECK(checker.CheckToken("0.2", IntegerToken{"0"}, 1));
        CHECK(checker.CheckToken("1234 + 0", IntegerToken{"1234"}, 4));
        CHECK(checker.CheckToken("1234+5", IntegerToken{"1234"}, 4));
        CHECK(checker.CheckNull(".2"));
        CHECK(checker.CheckNull(""));
        CHECK(checker.CheckNull("-2"));
        CHECK(checker.CheckNull("abc"));
    }

    std::set<std::string> keywords = {"let", "as", "where", "module", "import"};

    SECTION("Name Parser") {
        ParserChecker checker(NameKeywordParser{SearchSet{keywords}});
        CHECK(checker.CheckToken("variable", NameToken{"variable"}, 8));
        CHECK(checker.CheckToken("var23", NameToken{"var23"}, 5));
        CHECK(checker.CheckToken("_", NameToken{"_"}, 1));
        CHECK(checker.CheckToken("a", NameToken{"a"}, 1));
        CHECK(checker.CheckToken("a_b", NameToken{"a_b"}, 3));
        CHECK(checker.CheckToken("_c", NameToken{"_c"}, 2));
        CHECK(checker.CheckToken("d+5", NameToken{"d"}, 1));
        CHECK(checker.CheckToken("UPPERCASE_lowercase_Aa.", NameToken{"UPPERCASE_lowercase_Aa"}, 22)
        );
        CHECK(checker.CheckToken("a.b.c.d", NameToken{"a"}, 1));
        CHECK(checker.CheckNull(""));
        CHECK(checker.CheckNull("2var"));
        CHECK(checker.CheckNull("2_"));
        CHECK(checker.CheckNull("23"));
        CHECK(checker.CheckNull("2b"));
        CHECK(checker.CheckNull(".abc"));
        CHECK(checker.CheckNull("-abc"));
    }

    SECTION("Keyword Parser") {
        ParserChecker checker(NameKeywordParser{SearchSet{keywords}});
        CHECK(checker.CheckToken("abc", NameToken{"abc"}, 3));
        CHECK(checker.CheckToken("asap", NameToken{"asap"}, 4));
        for (const std::string& keyword : keywords) {
            CHECK(checker.CheckToken(keyword, KeywordToken{keyword}, keyword.size()));
        }
    }

    std::set<std::string> operators = {"+", "-", "*", "/", "^", "(", ")", ",", ".", ":="};

    SECTION("Operator Parser") {
        ParserChecker checker(OperatorParser{SearchSet{operators}});
        CHECK(checker.CheckNull(""));
        CHECK(checker.CheckNull("&+"));
        CHECK(checker.CheckNull("abc"));
        CHECK(checker.CheckNull("123"));
        CHECK(checker.CheckToken("++", OperatorToken{"+"}, 1));
        CHECK(checker.CheckToken("--", OperatorToken{"-"}, 1));
        CHECK(checker.CheckToken("+123", OperatorToken{"+"}, 1));
        CHECK(checker.CheckToken("-123", OperatorToken{"-"}, 1));
        CHECK(checker.CheckToken("+0", OperatorToken{"+"}, 1));
        CHECK(checker.CheckToken("-0", OperatorToken{"-"}, 1));
        for (const std::string& op : operators) {
            CHECK(checker.CheckToken(op, OperatorToken{op}, op.size()));
        }
    }

    SECTION("Whitespace Parser") {
        ParserChecker checker(WhitespaceParser{});
        CHECK(checker.CheckNull(""));
        CHECK(checker.CheckNull("abc"));
        CHECK(checker.CheckNull("123"));
        CHECK(checker.CheckNull("++"));
        CHECK(checker.CheckNull("."));
        CHECK(checker.CheckNull("abc "));
        CHECK(checker.CheckNull("abc\n"));
        CHECK(checker.CheckNull("abc\t"));
        CHECK(checker.CheckToken(" ", IndentToken{IndentType::SPACE}, 1));
        CHECK(checker.CheckToken("  ", IndentToken{IndentType::SPACE}, 1));
        CHECK(checker.CheckToken(" 123", IndentToken{IndentType::SPACE}, 1));
        CHECK(checker.CheckToken("\t", IndentToken{IndentType::TAB}, 1));
        CHECK(checker.CheckToken("\t\t", IndentToken{IndentType::TAB}, 1));
        CHECK(checker.CheckToken("\t456", IndentToken{IndentType::TAB}, 1));
        CHECK(checker.CheckToken(" \t", IndentToken{IndentType::SPACE}, 1));
        CHECK(checker.CheckToken("\t ", IndentToken{IndentType::TAB}, 1));
        CHECK(checker.CheckToken("\n", NewLineToken{}, 1));
        CHECK(checker.CheckToken("\n\n", NewLineToken{}, 1));
        CHECK(checker.CheckToken("\n   \t  ", NewLineToken{}, 1));
        CHECK(checker.CheckToken("   \t  \n", IndentToken{IndentType::SPACE}, 1));
    }
}

TEST_CASE("Tokenizer") {
    lex::Tokenizer tokenizer = fe::TokenizerFactory().Create();

    REQUIRE_NOTHROW(tokenizer.Tokenize(""));
    REQUIRE(tokenizer.Tokenize("").empty());

    REQUIRE_THROWS_AS(tokenizer.Tokenize("@"), lex::TokenizerUnknownSequence);
    REQUIRE_THROWS_AS(tokenizer.Tokenize("~"), lex::TokenizerUnknownSequence);
    REQUIRE_THROWS_AS(tokenizer.Tokenize("!"), lex::TokenizerUnknownSequence);
    REQUIRE_THROWS_AS(tokenizer.Tokenize("&"), lex::TokenizerUnknownSequence);
    REQUIRE_THROWS_AS(tokenizer.Tokenize("abcdef + 2 + 3^4?"), lex::TokenizerUnknownSequence);

    SECTION("All types of tokens") {
        auto code = "abc as 3 + \n\t_";
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE(tokens.size() == 11);
        CHECK(CheckEquals(tokens[0], NameToken{"abc"}));
        CHECK(CheckEquals(tokens[1], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[2], KeywordToken{"as"}));
        CHECK(CheckEquals(tokens[3], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[4], IntegerToken{"3"}));
        CHECK(CheckEquals(tokens[5], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[6], OperatorToken{"+"}));
        CHECK(CheckEquals(tokens[7], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[8], NewLineToken{}));
        CHECK(CheckEquals(tokens[9], IndentToken{IndentType::TAB}));
        CHECK(CheckEquals(tokens[10], NameToken{"_"}));
    }

    SECTION("Basic arithmetic") {
        auto code = "1+2-3*4/5^6";
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE(tokens.size() == 11);
        CHECK(CheckEquals(tokens[0], IntegerToken{"1"}));
        CHECK(CheckEquals(tokens[1], OperatorToken{"+"}));
        CHECK(CheckEquals(tokens[2], IntegerToken{"2"}));
        CHECK(CheckEquals(tokens[3], OperatorToken{"-"}));
        CHECK(CheckEquals(tokens[4], IntegerToken{"3"}));
        CHECK(CheckEquals(tokens[5], OperatorToken{"*"}));
        CHECK(CheckEquals(tokens[6], IntegerToken{"4"}));
        CHECK(CheckEquals(tokens[7], OperatorToken{"/"}));
        CHECK(CheckEquals(tokens[8], IntegerToken{"5"}));
        CHECK(CheckEquals(tokens[9], OperatorToken{"^"}));
        CHECK(CheckEquals(tokens[10], IntegerToken{"6"}));
    }

    SECTION("Indents") {
        auto code = "\t  a \t3";
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE(tokens.size() == 7);
        CHECK(CheckEquals(tokens[0], IndentToken{IndentType::TAB}));
        CHECK(CheckEquals(tokens[1], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[2], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[3], NameToken{"a"}));
        CHECK(CheckEquals(tokens[4], IndentToken{IndentType::SPACE}));
        CHECK(CheckEquals(tokens[5], IndentToken{IndentType::TAB}));
        CHECK(CheckEquals(tokens[6], IntegerToken{"3"}));
    }
}

TEST_CASE("TokensPreprocessor") {
    lex::Tokenizer tokenizer = fe::TokenizerFactory().Create();
    TokensPreprocessor tokens_preprocessor;

    SECTION("LineIndent") {
        SECTION("IsNormal") {
            CHECK(LineIndent{0, 0}.IsNormal());
            CHECK(LineIndent{1, 0}.IsNormal());
            CHECK(LineIndent{4, 0}.IsNormal());
            CHECK(LineIndent{0, 1}.IsNormal());
            CHECK(LineIndent{0, 4}.IsNormal());
            CHECK(!LineIndent{1, 1}.IsNormal());
            CHECK(!LineIndent{4, 4}.IsNormal());
            CHECK(!LineIndent{4, 1}.IsNormal());
        }

        SECTION("IsCorrectWith") {
            CHECK(LineIndent{0, 0}.IsCorrectWith({0, 0}));
            CHECK(LineIndent{0, 0}.IsCorrectWith({4, 0}));
            CHECK(LineIndent{0, 0}.IsCorrectWith({0, 1}));
            CHECK(LineIndent{4, 0}.IsCorrectWith({0, 0}));
            CHECK(LineIndent{0, 1}.IsCorrectWith({0, 0}));
            CHECK(!LineIndent{0, 1}.IsCorrectWith({1, 0}));
            CHECK(!LineIndent{1, 0}.IsCorrectWith({0, 1}));
            CHECK(!LineIndent{4, 1}.IsCorrectWith({1, 0}));
            CHECK(!LineIndent{1, 0}.IsCorrectWith({4, 1}));
            CHECK(!LineIndent{4, 1}.IsCorrectWith({1, 2}));
            CHECK(!LineIndent{1, 2}.IsCorrectWith({4, 1}));
        }

        SECTION("IsLess") {
            REQUIRE_THROWS_AS((LineIndent{0, 1}.IsLess({1, 0})), DifferentIndentTypesError);
            CHECK(LineIndent{0, 1}.IsLess({0, 10}));
            CHECK(!LineIndent{0, 7}.IsLess({0, 3}));
            CHECK(LineIndent{1, 0}.IsLess({10, 0}));
            CHECK(!LineIndent{7, 0}.IsLess({3, 0}));
        }

        SECTION("IsGreater") {
            REQUIRE_THROWS_AS((LineIndent{0, 1}.IsGreater({1, 0})), MixedIndentError);
            CHECK(!LineIndent{0, 1}.IsGreater({0, 10}));
            CHECK(LineIndent{0, 7}.IsGreater({0, 3}));
            CHECK(LineIndent{0, 3}.IsGreater({0, 2}));
            CHECK(LineIndent{0, 2}.IsGreater({0, 1}));
            CHECK(!LineIndent{1, 0}.IsGreater({10, 0}));
            CHECK(LineIndent{7, 0}.IsGreater({3, 0}));
            CHECK(!LineIndent{2, 0}.IsGreater({1, 0}));
            CHECK(LineIndent{3, 0}.IsGreater({1, 0}));
            CHECK(LineIndent{2, 0}.IsGreater({0, 0}));
            CHECK(!LineIndent{1, 0}.IsGreater({0, 0}));
            CHECK(LineIndent{0, 1}.IsGreater({0, 0}));
        }
    }

    auto get_tokens = [&](const std::string& code) {
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE_NOTHROW(tokens_preprocessor.Process(tokens));
        return tokens_preprocessor.Process(tokens);
    };

    auto require_error = [&](const std::string& code) {
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE_THROWS(tokens_preprocessor.Process(tokens));
    };

    SECTION("Empty line") {
        CHECK(get_tokens("").empty());
        CHECK(get_tokens("  ").empty());
        CHECK(get_tokens("\t\t").empty());
        CHECK(get_tokens("\n").empty());
        CHECK(get_tokens("  \n").empty());
        CHECK(get_tokens("\t\t\n").empty());
        CHECK(get_tokens("     \t\t\t  \t \n").empty());
        CHECK(get_tokens("\n\n\n").empty());
    }

    SECTION("No inner indents") {
        auto tokens = get_tokens("1   +\t\t 2\t  -    3   *   4   /  5 ^  6    ");
        REQUIRE(tokens.size() == 12);
        CHECK(CheckEquals(tokens[0], IntegerToken{"1"}));
        CHECK(CheckEquals(tokens[1], OperatorToken{"+"}));
        CHECK(CheckEquals(tokens[2], IntegerToken{"2"}));
        CHECK(CheckEquals(tokens[3], OperatorToken{"-"}));
        CHECK(CheckEquals(tokens[4], IntegerToken{"3"}));
        CHECK(CheckEquals(tokens[5], OperatorToken{"*"}));
        CHECK(CheckEquals(tokens[6], IntegerToken{"4"}));
        CHECK(CheckEquals(tokens[7], OperatorToken{"/"}));
        CHECK(CheckEquals(tokens[8], IntegerToken{"5"}));
        CHECK(CheckEquals(tokens[9], OperatorToken{"^"}));
        CHECK(CheckEquals(tokens[10], IntegerToken{"6"}));
        CHECK(CheckEquals(tokens[11], NewLineToken{}));
    }

    SECTION("Multiple lines") {
        auto tokens = get_tokens("\n1\n2\n\n3\n\n\n4\n5\n\n");
        REQUIRE(tokens.size() == 10);
        CHECK(CheckEquals(tokens[0], IntegerToken{"1"}));
        CHECK(CheckEquals(tokens[1], NewLineToken{}));
        CHECK(CheckEquals(tokens[2], IntegerToken{"2"}));
        CHECK(CheckEquals(tokens[3], NewLineToken{}));
        CHECK(CheckEquals(tokens[4], IntegerToken{"3"}));
        CHECK(CheckEquals(tokens[5], NewLineToken{}));
        CHECK(CheckEquals(tokens[6], IntegerToken{"4"}));
        CHECK(CheckEquals(tokens[7], NewLineToken{}));
        CHECK(CheckEquals(tokens[8], IntegerToken{"5"}));
        CHECK(CheckEquals(tokens[9], NewLineToken{}));
    }

    SECTION("Simple indent") {
        for (const std::string& code :
             std::vector{"\t1 + 2", "  1\t+\t2", "\t1 + 2\n", "  1\t+\t2\n"}) {
            auto tokens = get_tokens(code);
            REQUIRE(tokens.size() == 7);
            CHECK(CheckEquals(tokens[0], OpenBracketToken{}));
            CHECK(CheckEquals(tokens[1], IntegerToken{"1"}));
            CHECK(CheckEquals(tokens[2], OperatorToken{"+"}));
            CHECK(CheckEquals(tokens[3], IntegerToken{"2"}));
            CHECK(CheckEquals(tokens[4], NewLineToken{}));
            CHECK(CheckEquals(tokens[5], CloseBracketToken{}));
            CHECK(CheckEquals(tokens[6], NewLineToken{}));
        }
    }

    SECTION("Different indents") {
        for (const std::string& code : std::vector{"1\n\t2\n3", "1\n   2\n3", "1\n   2    \t\n3"}) {
            auto tokens = get_tokens(code);
            REQUIRE(tokens.size() == 8);
            CHECK(CheckEquals(tokens[0], IntegerToken{"1"}));
            CHECK(CheckEquals(tokens[1], OpenBracketToken{}));
            CHECK(CheckEquals(tokens[2], IntegerToken{"2"}));
            CHECK(CheckEquals(tokens[3], NewLineToken{}));
            CHECK(CheckEquals(tokens[4], CloseBracketToken{}));
            CHECK(CheckEquals(tokens[5], NewLineToken{}));
            CHECK(CheckEquals(tokens[6], IntegerToken{"3"}));
            CHECK(CheckEquals(tokens[7], NewLineToken{}));
        }
    }

    SECTION("Nested blocks") {
        for (const std::string& code : std::vector{
                 "1\n  2\n    3\n  4\n5\n", "1\n\t2\n\t\t3\n\t4\n5\n",
                 "1\n    2\n      3\n    4\n5\n"
             }) {
            auto tokens = get_tokens(code);
            REQUIRE(tokens.size() == 14);
            CHECK(CheckEquals(tokens[0], IntegerToken{"1"}));
            CHECK(CheckEquals(tokens[1], OpenBracketToken{}));
            CHECK(CheckEquals(tokens[2], IntegerToken{"2"}));
            CHECK(CheckEquals(tokens[3], OpenBracketToken{}));
            CHECK(CheckEquals(tokens[4], IntegerToken{"3"}));
            CHECK(CheckEquals(tokens[5], NewLineToken{}));
            CHECK(CheckEquals(tokens[6], CloseBracketToken{}));
            CHECK(CheckEquals(tokens[7], NewLineToken{}));
            CHECK(CheckEquals(tokens[8], IntegerToken{"4"}));
            CHECK(CheckEquals(tokens[9], NewLineToken{}));
            CHECK(CheckEquals(tokens[10], CloseBracketToken{}));
            CHECK(CheckEquals(tokens[11], NewLineToken{}));
            CHECK(CheckEquals(tokens[12], IntegerToken{"5"}));
            CHECK(CheckEquals(tokens[13], NewLineToken{}));
        }
    }

    SECTION("Errors") {
        require_error(" a");
        require_error("  a\n\tb");
        require_error("    a\n\tb");
        require_error("a\n  b\n\tc\n  d\ne");
        require_error("a\n\tb\n  c\n\td\ne");
    }
}

TEST_CASE("Grammar rule classes") {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::OptionalRule;
    using grammar_rules::RepeatRule;
    using grammar_rules::SequenceRule;
    using grammar_rules::TokenTypeRule;
    using grammar_rules::TokenValueRule;
    using grammar_rules::VariantRule;
    using grammar_rules::VirtualRules;

    using syntax_tree::NamedNode;
    using syntax_tree::OptionalNode;
    using syntax_tree::RepeatNode;
    using syntax_tree::SequenceNode;
    using syntax_tree::TokenNode;
    using syntax_tree::VariantNode;

    REQUIRE_THROWS_AS(
        syntax_tree::Cast<SequenceNode>(syntax_tree::MakeNode<TokenNode>(MakeToken<IntegerToken>("1"
        ))),
        syntax_tree::ImpossibleNodeCastError
    );
    REQUIRE_THROWS_AS(
        syntax_tree::Cast<SequenceNode>(nullptr), syntax_tree::ImpossibleNodeCastError
    );

    SECTION("Grammar") {
        Grammar grammar;
        REQUIRE_THROWS_AS(grammar.AddRule("", nullptr), EmptyRuleError);
        REQUIRE_NOTHROW(grammar.AddRule("abc", MakeRule<TokenTypeRule<NameToken>>()));
        REQUIRE_THROWS_AS(
            grammar.AddRule("abc", MakeRule<TokenTypeRule<NameToken>>()), RuleAlreadyExistsError
        );
    }

    SECTION("TokenTypeRule") {
        CHECK(CheckEmptyNode("", MakeRule<TokenTypeRule<NameToken>>()));
        CHECK(CheckNodeType<TokenNode>("a", MakeRule<TokenTypeRule<NameToken>>()));
        CHECK(CheckNodeType<TokenNode>("a b", MakeRule<TokenTypeRule<NameToken>>()));
        CHECK(CheckNodeType<TokenNode>("a 2", MakeRule<TokenTypeRule<NameToken>>()));
        CHECK(CheckEmptyNode("as", MakeRule<TokenTypeRule<NameToken>>()));
        CHECK(CheckEmptyNode("1", MakeRule<TokenTypeRule<NameToken>>()));
    }

    SECTION("TokenValueRule") {
        CHECK(CheckEmptyNode("", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckNodeType<TokenNode>("a", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckNodeType<TokenNode>("a +", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckEmptyNode("b", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckEmptyNode("as", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckEmptyNode("as", MakeRule<TokenValueRule<NameToken>>("as")));
        CHECK(CheckNodeType<TokenNode>("as", MakeRule<TokenValueRule<KeywordToken>>("as")));
        CHECK(CheckEmptyNode("1", MakeRule<TokenValueRule<NameToken>>("a")));
        CHECK(CheckEmptyNode("1", MakeRule<TokenValueRule<NameToken>>("def")));
    }

    SECTION("NamedRule") {
        Grammar grammar;
        grammar.AddRule("my_rule", MakeRule<TokenTypeRule<IntegerToken>>());
        CHECK(CheckEmptyNode("", MakeRule<NamedRule>("my_rule"), grammar));
        CHECK(CheckNodeType<NamedNode>("123", MakeRule<NamedRule>("my_rule"), grammar));
        CHECK(CheckNodeType<NamedNode>("123 as", MakeRule<NamedRule>("my_rule"), grammar));
        CHECK(CheckEmptyNode("abc", MakeRule<NamedRule>("my_rule"), grammar));
        REQUIRE_THROWS_AS(
            CheckNodeType<NamedNode>("123", MakeRule<NamedRule>("my_rule")), EmptyRuleError
        );
    }

    SECTION("SequenceRule") {
        auto rule = MakeRule<SequenceRule>(
            {MakeRule<TokenTypeRule<NameToken>>(), MakeRule<TokenValueRule<IntegerToken>>("1")}
        );
        CHECK(CheckEmptyNode("", rule));
        CHECK(CheckEmptyNode("1", rule));
        CHECK(CheckEmptyNode("a1", rule));
        CHECK(CheckNodeType<SequenceNode>("a 1", rule));
        CHECK(CheckEmptyNode("a 2", rule));
        CHECK(CheckNodeType<SequenceNode>("a 1 bcd 123", rule));
        CHECK(CheckNodeType<SequenceNode>("bcd 1 a", rule));
        CHECK(CheckNodeType<SequenceNode>("bcd 123 a", MakeRule<SequenceRule>({})));
        CHECK(CheckNodeType<SequenceNode>("", MakeRule<SequenceRule>({})));
    }

    SECTION("VariantRule") {
        auto rule = MakeRule<VariantRule>(
            {MakeRule<TokenTypeRule<NameToken>>(), MakeRule<TokenValueRule<IntegerToken>>("1")}
        );
        CHECK(CheckEmptyNode("", rule));
        CHECK(CheckNodeType<VariantNode>("1", rule));
        CHECK(CheckNodeType<VariantNode>("a1", rule));
        CHECK(CheckNodeType<VariantNode>("a 1", rule));
        CHECK(CheckNodeType<VariantNode>("1 1", rule));
        CHECK(CheckEmptyNode("+ 1", rule));
        CHECK(CheckEmptyNode("a", MakeRule<VariantRule>({})));
        CHECK(CheckEmptyNode("", MakeRule<VariantRule>({})));
    }

    SECTION("OptionalRule") {
        auto rule = MakeRule<OptionalRule>(MakeRule<TokenTypeRule<OperatorToken>>());
        REQUIRE(RequireNode<OptionalNode>("+", rule)->HasNode());
        REQUIRE(!RequireNode<OptionalNode>("abc", rule)->HasNode());
        REQUIRE(!RequireNode<OptionalNode>("1", rule)->HasNode());
    }
}

TEST_CASE("Simple grammar parsing") {
    using grammar_rules::MakeRule;
    using grammar_rules::NamedRule;
    using grammar_rules::OptionalRule;
    using grammar_rules::SequenceRule;
    using grammar_rules::TokenValueRule;

    using syntax_tree::NamedNode;
    using syntax_tree::OptionalNode;
    using syntax_tree::SequenceNode;

    Grammar grammar;
    grammar.AddRule(
        "E", MakeRule<OptionalRule>(MakeRule<SequenceRule>(
                 {MakeRule<TokenValueRule<OperatorToken>>("("), MakeRule<NamedRule>("E"),
                  MakeRule<TokenValueRule<OperatorToken>>(")"), MakeRule<NamedRule>("E")}
             ))
    );

    REQUIRE_THROWS_AS(Parser(grammar).Parse({}), MainRuleDoesNotExistError);

    grammar.SetMainRule("E");
    Parser parser(grammar);

    auto get_tokens = [&](const std::string& code) {
        lex::Tokenizer tokenizer = fe::TokenizerFactory().Create();
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        return parser.Parse(tokens);
    };

    auto named = [&](syntax_tree::NodePtr root) {
        return syntax_tree::Cast<NamedNode>(root)->GetChildren().at(0);
    };
    auto optional = [&](syntax_tree::NodePtr root, bool has_node) -> syntax_tree::NodePtr {
        auto node = syntax_tree::Cast<OptionalNode>(root);
        REQUIRE(node->HasNode() == has_node);
        if (node->HasNode()) {
            return node->GetChildren().at(0);
        }
        return nullptr;
    };
    auto sequence = [&](syntax_tree::NodePtr root) {
        return syntax_tree::Cast<SequenceNode>(root)->GetChildren();
    };

    SECTION("Extra tokens") {
        REQUIRE_THROWS_AS(get_tokens("+"), ExtraTokensInCodeError);
        REQUIRE_THROWS_AS(get_tokens("("), ExtraTokensInCodeError);
        REQUIRE_THROWS_AS(get_tokens(")"), ExtraTokensInCodeError);
        REQUIRE_THROWS_AS(get_tokens("(()"), ExtraTokensInCodeError);
        REQUIRE_THROWS_AS(get_tokens("()a"), ExtraTokensInCodeError);
    }

    SECTION("Empty") {
        auto tree = get_tokens("");
        optional(tree, false);
    }

    SECTION("Single") {
        auto tree = get_tokens("()");
        auto nodes = sequence(optional(tree, true));
        optional(named(nodes.at(1)), false);
        optional(named(nodes.at(3)), false);
    }

    SECTION("Complex") {
        auto tree = get_tokens("(())()()");
        auto nodes1 = sequence(optional(tree, true));

        auto nodes2 = sequence(optional(named(nodes1.at(1)), true));
        optional(named(nodes2.at(1)), false);
        optional(named(nodes2.at(3)), false);

        auto nodes3 = sequence(optional(named(nodes1.at(3)), true));
        optional(named(nodes3.at(1)), false);

        auto nodes4 = sequence(optional(named(nodes3.at(3)), true));
        optional(named(nodes4.at(1)), false);
        optional(named(nodes4.at(3)), false);
    }
}

TEST_CASE("Building and formatting") {
    auto format = [](const std::string& code) {
        lex::Tokenizer tokenizer = fe::TokenizerFactory().Create();
        lex::Tokens tokens = tokenizer.Tokenize(code);

        TokensPreprocessor preprocessor;
        Tokens new_tokens = preprocessor.Process(tokens);

        Parser parser = fe::ParserFactory().Create();
        syntax_tree::NodePtr syntax_tree = parser.Parse(new_tokens);

        if (!syntax_tree) {
            throw std::runtime_error{"Can't process empty syntax tree."};
        }

        ast::FullAstBuilder ast_builder;
        ast::NodePtr ast_result = ast_builder.Build(syntax_tree);

        if (!ast_result) {
            throw std::runtime_error{"Can't process empty AST."};
        }

        ast::FormatVisitor visitor;
        ast_result->Accept(visitor);
        return visitor.GetResult();
    };

    SECTION("Variable definition") {
        REQUIRE(format("let     a    :=    0") == "let a := 0\n");
        REQUIRE(format("let   b   :=   a") == "let b := a\n");
        REQUIRE(format("let   c   :=   a         .  b  .  c   .   d") == "let c := a.b.c.d\n");
        REQUIRE(format("let   d   :=   b ( )\n\n\n") == "let d := b()\n");
        REQUIRE(format("let   e0   :=   hello.world ()\n\n\n") == "let e0 := hello.world()\n");
    }

    SECTION("Function definition") {
        REQUIRE(format("let func (  ) := f()\n") == "let func() := f()\n");
        REQUIRE(
            format("let func (  a , b    ,c) := h   ( f (  a  ) ,    g ( b ) ) \n") ==
            "let func(a, b, c) := h(f(a), g(b))\n"
        );
        REQUIRE(
            format("let sum (lhs ,rhs) :=       lhs      +       rhs    \n") ==
            "let sum(lhs, rhs) := lhs + rhs\n"
        );
    }

    SECTION("Arithmetic expressions") {
        REQUIRE(format("let negative_number := -     2\n") == "let negative_number := -2\n");
        REQUIRE(format("let a0_ := 1  +  2\n") == "let a0_ := 1 + 2\n");
        REQUIRE(format("let _ := 1 + 2 + 3 + 4 + 5\n") == "let _ := 1 + 2 + 3 + 4 + 5\n");
        REQUIRE(format("let _ := ((((1 + 2) + 3) + 4) + 5)\n") == "let _ := 1 + 2 + 3 + 4 + 5\n");
        REQUIRE(format("let _ := (1 + (2 + (3 + (4 + 5))))\n") == "let _ := 1 + 2 + 3 + 4 + 5\n");
        REQUIRE(
            format("let _ := 1 + (2 * 3) + 4 ^ ((((7)))) - ((3 / 7) ^ 5)\n") ==
            "let _ := 1 + 2 * 3 + 4 ^ 7 - (3 / 7) ^ 5\n"
        );
        REQUIRE(
            format("let _ := 1 + (2 * 3) + 4 ^ ((((7)))) - ((3 / 7) ^ 5)\n") ==
            "let _ := 1 + 2 * 3 + 4 ^ 7 - (3 / 7) ^ 5\n"
        );
        REQUIRE(format("let _ := 1--1\n") == "let _ := 1 - (-1)\n");
        REQUIRE(
            format("let _ := f(-1,1-1,-1-1,1+-1)") == "let _ := f(-1, 1 - 1, -1 - 1, 1 + (-1))\n"
        );
    }

    SECTION("Define module") {
        REQUIRE(format("module a where\n\t\t\t\tlet b := c\n") == "module a where\n  let b := c\n");
        REQUIRE(format("module a where\n  let b := c\n") == "module a where\n  let b := c\n");
        REQUIRE(
            format("let c(a, b) := d where    \n\t\t  \n  let b := c\n") ==
            "let c(a, b) := d where\n  let b := c\n"
        );
    }

    SECTION("Imports") {
        REQUIRE(format("import c\nimport b\nimport a") == "import a\nimport b\nimport c\n");
        REQUIRE(
            format("import a . b . c  \nimport a.b   \nimport a") ==
            "import a\nimport a.b\nimport a.b.c\n"
        );
        REQUIRE(
            format("import a . b . c  \nimport a.b   \nimport a") ==
            "import a\nimport a.b\nimport a.b.c\n"
        );
        REQUIRE(
            format("import a as C  \nimport a as B   \nimport a as A") ==
            "import a as A\nimport a as B\nimport a as C\n"
        );
        REQUIRE(
            format("import a (b, c, d)  \nimport a (b, a, d)   \nimport a as A") ==
            "import a as A\nimport a (a, b, c, d)\n"
        );
    }

    SECTION("Large examples") {
        REQUIRE(
            format("module MyLinalg where\n"
                   "   import Unused\n"
                   "\n"
                   "   let    d    :=        e()\n"
                   "   let test1 := a    .   b     . c . d  (a.d.e.f  ,       2      ,      3, -   "
                   " 4, (aaa.bbb.c.d))\n"
                   "   let test2 := 1 + 2 - 3 + 4 * 5 + 2 ^ 3 ^ 4 + 10 + (20 + 30 / 2)\n"
                   "   let test3 := ((1 + 2) + (3 + 4)) + (1 - 2) - (3 - 4)\n"
                   "   let test4 := a(b,c.d.e+1--2*3/4^5)\n"
                   "   let test5(   aaaa, b,       d, a0) := 1 + 2 + 3\n"
                   "\n"
                   "   let solve(A, b  )   :=   LA.solve( A  ,  b  )\n"
                   "\n"
                   "   import numpy.linalg as LA\n"
                   "\n"
                   "\n"
                   "let some_root(a , b  , c  ) :=   (  b +   math.sqrt(( discriminant(a, b, c ) ) "
                   ")) / a where\n"
                   "\n"
                   "  let discriminant(   a, b,c   ) := (b ^ 2) - 4 * (a * c)\n"
                   "\n"
                   "\n"
                   "  import math") ==
            "module MyLinalg where\n"
            "  import Unused\n"
            "  import numpy.linalg as LA\n"
            "  let d := e()\n"
            "  let test1 := a.b.c.d(a.d.e.f, 2, 3, -4, aaa.bbb.c.d)\n"
            "  let test2 := 1 + 2 - 3 + 4 * 5 + 2 ^ 3 ^ 4 + 10 + 20 + 30 / 2\n"
            "  let test3 := 1 + 2 + 3 + 4 + 1 - 2 - (3 - 4)\n"
            "  let test4 := a(b, c.d.e + 1 - (-2) * 3 / 4 ^ 5)\n"
            "  let test5(aaaa, b, d, a0) := 1 + 2 + 3\n"
            "  let solve(A, b) := LA.solve(A, b)\n"
            "\n"
            "let some_root(a, b, c) := (b + math.sqrt(discriminant(a, b, c))) / a where\n"
            "  import math\n"
            "  let discriminant(a, b, c) := b ^ 2 - 4 * a * c\n"
        );

        REQUIRE(
            format("import M2 as abc\n"
                   "import M2 as cba\n"
                   "import M1 (f1, f2, f4, f6)\n"
                   "import M1.M2 (f3, f5)\n"
                   "\n"
                   "module MyLinalg where\n"
                   "  import Unused\n"
                   "  import numpy.linalg as LA\n"
                   "  let x := -5\n"
                   "  let x := -5 + 5 + (-5) - (-5)\n"
                   "  let d := e()\n"
                   "  let test1 := a.b.c.d(a.d.e.f, 2, 3, -4, aaa.bbb.c.d)\n"
                   "  let test2 := 1 + 2 - 3 + 4 * 5 + 2 ^ 3 ^ 4 + 10 + 20 + 30 / 2\n"
                   "  let test3 := 1 + 2 + 3 + 4 + 1 - 2 - (3 - 4)\n"
                   "  let test4 := a(b, c.d.e + 1 - (-2) * 3 / 4 ^ 5)\n"
                   "  let test5(aaaa, b, d, a0) := 1 + 2 + 3\n"
                   "  let solve(A, b) := LA.solve(A, b)\n"
                   "\n"
                   "let some_root(a, b, c) := (b + math.sqrt(discriminant(a, b, c))) / a where\n"
                   "  import math\n"
                   "  let discriminant(a, b, c) := b ^ 2 - 4 * a * c\n"
                   "\n"
                   "module MyWorld where\n"
                   "  import a\n"
                   "  import b\n"
                   "  import c as d\n"
                   "\n"
                   "  let a() := 10 where\n"
                   "    let b := c\n"
                   "\n"
                   "  let b := -1 + func(-1 + (-1), -1 - (-1), -1 * (-1), -1 / (-1), -1 ^ (-1))") ==
            "import M2 as abc\n"
            "import M2 as cba\n"
            "import M1 (f1, f2, f4, f6)\n"
            "import M1.M2 (f3, f5)\n"
            "\n"
            "module MyLinalg where\n"
            "  import Unused\n"
            "  import numpy.linalg as LA\n"
            "  let x := -5\n"
            "  let x := -5 + 5 + (-5) - (-5)\n"
            "  let d := e()\n"
            "  let test1 := a.b.c.d(a.d.e.f, 2, 3, -4, aaa.bbb.c.d)\n"
            "  let test2 := 1 + 2 - 3 + 4 * 5 + 2 ^ 3 ^ 4 + 10 + 20 + 30 / 2\n"
            "  let test3 := 1 + 2 + 3 + 4 + 1 - 2 - (3 - 4)\n"
            "  let test4 := a(b, c.d.e + 1 - (-2) * 3 / 4 ^ 5)\n"
            "  let test5(aaaa, b, d, a0) := 1 + 2 + 3\n"
            "  let solve(A, b) := LA.solve(A, b)\n"
            "\n"
            "let some_root(a, b, c) := (b + math.sqrt(discriminant(a, b, c))) / a where\n"
            "  import math\n"
            "  let discriminant(a, b, c) := b ^ 2 - 4 * a * c\n"
            "\n"
            "module MyWorld where\n"
            "  import a\n"
            "  import b\n"
            "  import c as d\n"
            "\n"
            "  let a() := 10 where\n"
            "    let b := c\n"
            "\n"
            "  let b := -1 + func(-1 + (-1), -1 - (-1), -1 * (-1), -1 / (-1), -1 ^ (-1))\n"
        );
    }
}
