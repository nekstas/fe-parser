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
        core/tokenizer/token_types.h
        utils/istream.cpp
        utils/istream.h
        core/tokenizer/parsers/abstract_parser.h
        core/tokenizer/tokenizer_context.h
        core/tokenizer/tokenizer_factory.cpp
        core/tokenizer/tokenizer_factory.h
        core/tokenizer/parsers/integer_parser.cpp
        core/tokenizer/parsers/integer_parser.h
        core/tokenizer/parsers/non_empty_parser.cpp
        core/tokenizer/parsers/non_empty_parser.h
        utils/utils.cpp
        utils/utils.h
        utils/FormatStream.h
)
