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
        core/tokenizer/tokenizer_creator.cpp
        core/tokenizer/tokenizer_creator.h
        core/tokenizer/parsers/integer_parser.cpp
        core/tokenizer/parsers/integer_parser.h
        utils/utils.cpp
        utils/utils.h
        utils/FormatStream.h
        core/errors/syntax_errors.h
        core/errors/logic_errors.h
        core/tokenizer/tokens/space_token.h
        core/tokenizer/parsers/sequence_parsers.h
        core/common/code/code_stream.cpp
        core/common/code/code_stream.h
)
