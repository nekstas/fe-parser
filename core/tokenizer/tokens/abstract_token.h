#pragma once
#include "../token_types.h"

struct AbstractToken {
public:
    virtual ~AbstractToken() {
    }

    virtual TokenType GetType() const = 0;

    virtual std::string ToString() const = 0;
};

using Token = std::shared_ptr<AbstractToken>;

template <typename T, typename... Args>
Token MakeToken(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

inline std::ostream& operator<<(std::ostream& out, const Token& token) {
    return out << token->ToString();
}
