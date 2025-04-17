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

        return token && IsToken<TokenType>(token) &&
               *ConvertTokenTo<TokenType>(token) == expected_token &&
               stream.GetPos() == expected_pos;
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
    lex::Tokenizer tokenizer = fe::TokenizerFactory().Create("");
}