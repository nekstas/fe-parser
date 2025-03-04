#pragma once
#include <memory>
#include <string>

struct AbstractToken {
public:
    virtual ~AbstractToken() {
    }

    virtual std::string ToString() const = 0;
};

using Token = std::shared_ptr<AbstractToken>;

template <typename T, typename... Args>
Token MakeToken(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> ConvertTokenTo(Token token) {
    return std::dynamic_pointer_cast<T>(token);
}

template <typename T>
bool IsToken(Token token) {
    return static_cast<bool>(ConvertTokenTo<T>(token));
}

inline std::ostream& operator<<(std::ostream& out, const Token& token) {
    return out << token->ToString();
}
