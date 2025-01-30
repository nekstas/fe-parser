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
        core/tokenizer/tokens/base_token.h
        core/tokenizer/token_types.h)
