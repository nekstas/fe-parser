#pragma once

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

struct AbstractToken;
using Token = std::shared_ptr<AbstractToken>;

class ImpossibleTokenCastError : public std::runtime_error {
public:
    ImpossibleTokenCastError() : std::runtime_error{"Impossible to cast token to another type."} {
    }
};

struct AbstractToken {
public:
    virtual ~AbstractToken() {
    }

    virtual std::string ToString() const = 0;

public:
    static std::string GetStringRepresentation(Token token) {
        if (!token) {
            return "<No token>";
        }
        return token->ToString();
    }
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
std::shared_ptr<T> CastToken(Token token) {
    auto result = ConvertTokenTo<T>(token);
    if (!result) {
        throw ImpossibleTokenCastError{};
    }
    return result;
}

template <typename T>
bool IsToken(Token token) {
    return static_cast<bool>(ConvertTokenTo<T>(token));
}

inline std::ostream& operator<<(std::ostream& out, const Token& token) {
    return out << token->ToString();
}
