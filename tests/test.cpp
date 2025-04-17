#include <histedit.h>

#include <catch2/catch_test_macros.hpp>

#include "../app/fe/factories/tokenizer_factory.h"
#include "../app/fe/tokenizer/parsers/integer_parser.h"
#include "../app/fe/tokenizer/parsers/name_keyword_parser.h"
#include "../app/fe/tokenizer/parsers/operator_parser.h"
#include "../app/fe/tokenizer/parsers/whitespace_parser.h"
#include "../app/fe/tokenizer/tokens/indent_token.h"
#include "../app/fe/tokenizer/tokens/integer_token.h"
#include "../app/fe/tokenizer/tokens/keyword_token.h"
#include "../app/fe/tokenizer/tokens/name_token.h"
#include "../app/fe/tokenizer/tokens/new_line_token.h"
#include "../app/fe/tokens_preprocessor/tokens_preprocessor.h"

template <typename TokenType>
bool CheckEquals(Token token, const TokenType& expected) {
    return IsToken<TokenType>(token) && *ConvertTokenTo<TokenType>(token) == expected;
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

    SECTION("Basic arithmetic") {
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

    auto get_tokens = [&](const std::string& code) {
        REQUIRE_NOTHROW(tokenizer.Tokenize(code));
        auto tokens = tokenizer.Tokenize(code);
        REQUIRE_NOTHROW(tokens_preprocessor.Process(tokens));
        return tokens_preprocessor.Process(tokens);
    };

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
}