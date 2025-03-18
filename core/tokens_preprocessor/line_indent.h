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

    bool IsNormal() const;

    bool IsCorrectWith(const LineIndent& other) const;

    bool IsLess(const LineIndent& other) const;

    bool IsGreater(const LineIndent& other) const;

    bool operator==(const LineIndent& other) const;
};

std::ostream& operator<<(std::ostream& out, const LineIndent& line_indent);
