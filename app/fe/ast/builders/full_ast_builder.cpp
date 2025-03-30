#include "full_ast_builder.h"

ast::NodePtr ast::FullAstBuilder::Build(syntax_tree::NodePtr root) {
    return BuildModule(root);
}
