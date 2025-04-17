#include "application.h"

#include <iostream>

#include "../core/parser/parser.h"
#include "../core/tokenizer/tokenizer.h"
#include "../utils/utils.h"
#include "fe/ast/builders/full_ast_builder.h"
#include "fe/ast/visitors/format_visitor.h"
#include "fe/factories/parser_factory.h"
#include "fe/factories/tokenizer_factory.h"
#include "fe/tokens_preprocessor/tokens_preprocessor.h"

int32_t Application::Run(size_t argc, char** argv) {
    try {
        if (argc < kMinArgsCount || argc > kMaxArgsCount) {
            PrintHelpMessage();
        } else if (argc == kMinArgsCount) {
            FormatFile(argv[1], argv[1]);
        } else {
            FormatFile(argv[1], argv[2]);
        }
    } catch (std::runtime_error error) {
        std::cerr << "An error has occurred: " << error.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error has occurred.\n";
        return 2;
    }
    return 0;
}

void Application::FormatFile(const std::string& input_file, const std::string& output_file) {
    std::cout << "Reading input file...\n";
    std::string code = utils::ReadFile(input_file);

    std::cout << "Trying to tokenize input code...\n";
    lex::Tokenizer tokenizer = fe::TokenizerFactory().Create(code);
    lex::Tokens tokens = tokenizer.Tokenize();

    std::cout << "Preprocess input tokens...\n";
    TokensPreprocessor preprocessor(tokens);
    Tokens new_tokens = preprocessor.Process();

    std::cout << "Parsing tokens into syntax tree...\n";
    Parser parser = fe::ParserFactory().Create(new_tokens);
    syntax_tree::NodePtr syntax_tree = parser.Parse();

    if (!syntax_tree) {
        throw std::runtime_error{"Can't process empty syntax tree."};
    }

    std::cout << "Transform syntax tree to another AST...\n";
    ast::FullAstBuilder ast_builder;
    ast::NodePtr ast_result = ast_builder.Build(syntax_tree);

    if (!ast_result) {
        throw std::runtime_error{"Can't process empty AST."};
    }

    std::cout << "Formatting AST...\n";
    ast::FormatVisitor visitor;
    ast_result->Accept(visitor);
    std::string result = visitor.GetResult();

    std::cout << "Writing result into output file...\n";
    utils::WriteFile(output_file, visitor.GetResult());

    std::cout << "Done!\n";
}

void Application::PrintHelpMessage() {
    std::cout << "Parser for language described here: \n"
                 "https://gist.github.com/TurtlePU/0f74a0ad7783704e28bc08bc6bb95acb\n"
                 "Usage:"
                 "fe_parser <path_to_file_to_format>\n"
                 "fe_parser <input_file> <output_file>\n";
}
