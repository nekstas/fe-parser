#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

struct LineIndent;

class MixedIndentError : public std::runtime_error {
public:
    MixedIndentError(const LineIndent& indent);
};

class DifferentIndentTypesError : public std::runtime_error {
public:
    DifferentIndentTypesError(const LineIndent& indent1, const LineIndent& indent2);
};

class IndentsMismatchError : public std::runtime_error {
public:
    IndentsMismatchError(const LineIndent& indent1, const LineIndent& indent2);
};

struct LineIndent {
private:
    static constexpr size_t kMinSpacesDiff = 2;
    static constexpr size_t kMinTabsDiff = 1;

public:
    size_t spaces{0};
    size_t tabs{0};

    bool IsNormal() const {
        return !(spaces > 0 && tabs > 0);
    }

    bool IsCorrectWith(const LineIndent& other) const {
        return !(spaces > 0 && other.tabs > 0 || tabs > 0 && other.spaces > 0);
    }

    bool IsLess(const LineIndent& other) const {
        if (!IsCorrectWith(other)) {
            throw DifferentIndentTypesError(*this, other);
        }

        return spaces < other.spaces || tabs < other.tabs;
    }

    bool IsGreater(const LineIndent& other) const {
        if (!IsCorrectWith(other)) {
            throw MixedIndentError(*this);
        }

        return spaces >= other.spaces + kMinSpacesDiff || tabs >= other.tabs + kMinTabsDiff;
    }

    bool operator==(const LineIndent& other) const {
        return spaces == other.spaces && tabs == other.tabs;
    }
};

inline std::ostream& operator<<(std::ostream& out, const LineIndent& line_indent) {
    out << "LineIndent[spaces=" << line_indent.spaces << ", tabs=" << line_indent.tabs << "]";
    return out;
}
