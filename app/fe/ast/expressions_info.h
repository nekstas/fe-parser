#pragma once

#include <cstdint>
#include <exception>
#include <map>
#include <stdexcept>

#include "../../../utils/format_stream.h"

namespace ast {

class AddExtraExpressionInfoError : public std::runtime_error {
public:
    AddExtraExpressionInfoError(const std::string& code)
        : std::runtime_error{
              FormatStream() << "Attempt to add extra expression info (for operator \"" << code
                             << "\")."
          } {
    }
};

class ExpressionInfoDoesNotExistError : public std::runtime_error {
public:
    ExpressionInfoDoesNotExistError(const std::string& code)
        : std::runtime_error{
              FormatStream()
              << "Attempt to get expression info, that does not exist (for operator \"" << code
              << "\")."
          } {
    }
};

class ExpressionInfoInfoMergeError : public std::runtime_error {
public:
    ExpressionInfoInfoMergeError()
        : std::runtime_error{FormatStream() << "Can't merge these two expression infos"} {
    }
};

class ExpressionsInfo {
public:
    enum class Associativity { UNKNOWN, LEFT, RIGHT, BOTH };

    static bool IsCompatible(Associativity lhs, Associativity rhs) {
        if (lhs == Associativity::UNKNOWN || rhs == Associativity::UNKNOWN) {
            return false;
        }

        return lhs == rhs || lhs == Associativity::BOTH || rhs == Associativity::BOTH;
    }

    struct Info {
        Associativity associativity;
        size_t priority;

        Info() : associativity(Associativity::UNKNOWN), priority(0) {
        }

        Info(Associativity associativity, size_t priority)
            : associativity(associativity), priority(priority) {
        }

        Info Merge(const Info& other) {
            if (associativity == Associativity::UNKNOWN) {
                return other;
            }

            if (priority != other.priority) {
                throw ExpressionInfoInfoMergeError{};
            }

            if (associativity == Associativity::BOTH) {
                return {other.associativity, priority};
            } else if (other.associativity == Associativity::BOTH) {
                return {associativity, priority};
            } else if (associativity != other.associativity) {
                throw ExpressionInfoInfoMergeError{};
            }

            return {associativity, priority};
        }

        bool operator==(const Info& other) const {
            return associativity == other.associativity && priority == other.priority;
        }
    };

public:
    ExpressionsInfo() = default;

    void AddInfo(const std::string& code, const Info& new_info);

    Info GetInfo(const std::string& code) const;

private:
    std::map<std::string, Info> info_;
};

};  // namespace ast
