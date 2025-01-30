#include <vector>

struct TokenType {};

using Tokens = std::vector<TokenType>;

class Tokenizer {
public:
    Tokenizer(std::istream& input);

    Tokens Tokenize();
};