cmake_minimum_required(VERSION 3.29)
project(fe_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(fe_parser main.cpp
        app/application.cpp
        app/application.h
        core/tokenizer/tokenizer.cpp
        core/tokenizer/tokenizer.h
        core/tokenizer/tokens/integer_token.h
        core/tokenizer/tokens/operator_token.h
        core/tokenizer/tokens/abstract_token.h
        utils/istream.cpp
        utils/istream.h
        core/tokenizer/parsers/abstract_parser.h
        core/tokenizer/tokenizer_creator.cpp
        core/tokenizer/tokenizer_creator.h
        core/tokenizer/parsers/integer_parser.cpp
        core/tokenizer/parsers/integer_parser.h
        utils/utils.cpp
        utils/utils.h
        utils/format_stream.h
        core/tokenizer/tokens/indent_token.h
        core/tokenizer/parsers/operator_parser.cpp
        core/tokenizer/parsers/operator_parser.h
        core/algo/search_set.hpp
        core/tokenizer/parsers/whitespace_parser.cpp
        core/tokenizer/parsers/name_parser.h
        core/tokenizer/parsers/name_parser.cpp
        core/tokenizer/tokens/name_token.h
        core/tokens_preprocessor/tokens_preprocessor.cpp
        core/tokens_preprocessor/line_indent.h
        utils/container_stream.hpp
        utils/useful_streams.h
        core/tokens_preprocessor/line_indent.cpp
        core/tokenizer/tokens/open_bracket.h
        core/tokenizer/tokens/close_bracket.h
)
