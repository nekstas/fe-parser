#include "line_indent.h"

#include "../../utils/format_stream.h"

MixedIndentError::MixedIndentError(const LineIndent& indent)
    : std::runtime_error(FormatStream() << "Mixed " << indent << " can't be used.") {
}

DifferentIndentTypesError::DifferentIndentTypesError(const LineIndent& indent1,
                                                     const LineIndent& indent2)
    : std::runtime_error(FormatStream() << indent1 << ", " << indent2
                                        << ". Spaces and tabs can't be used together.") {
}

IndentsMismatchError::IndentsMismatchError(const LineIndent& indent1, const LineIndent& indent2)
    : std::runtime_error(FormatStream()
                         << indent1 << ", " << indent2 << ". Indents should be the same.") {
}
