#include "line_indent.h"

#include "../../utils/format_stream.h"

MixedIndentError::MixedIndentError(const LineIndent& indent)
    : std::runtime_error(FormatStream() << "Mixed " << indent << " can't be used.") {
}

DifferentIndentTypesError::DifferentIndentTypesError(
    const LineIndent& indent1, const LineIndent& indent2
)
    : std::runtime_error(
          FormatStream() << indent1 << ", " << indent2
                         << ". Spaces and tabs can't be used together."
      ) {
}

IndentsMismatchError::IndentsMismatchError(const LineIndent& indent1, const LineIndent& indent2)
    : std::runtime_error(
          FormatStream() << indent1 << ", " << indent2 << ". Indents should be the same."
      ) {
}

bool LineIndent::IsNormal() const {
    return !(spaces > 0 && tabs > 0);
}

bool LineIndent::IsCorrectWith(const LineIndent& other) const {
    return !(spaces > 0 && other.tabs > 0 || tabs > 0 && other.spaces > 0);
}

bool LineIndent::IsLess(const LineIndent& other) const {
    if (!IsCorrectWith(other)) {
        throw DifferentIndentTypesError(*this, other);
    }

    return spaces < other.spaces || tabs < other.tabs;
}

bool LineIndent::IsGreater(const LineIndent& other) const {
    if (!IsCorrectWith(other)) {
        throw MixedIndentError(*this);
    }

    return spaces >= other.spaces + kMinSpacesDiff || tabs >= other.tabs + kMinTabsDiff;
}

bool LineIndent::operator==(const LineIndent& other) const {
    return spaces == other.spaces && tabs == other.tabs;
}

std::ostream& operator<<(std::ostream& out, const LineIndent& line_indent) {
    out << "LineIndent[spaces=" << line_indent.spaces << ", tabs=" << line_indent.tabs << "]";
    return out;
}
