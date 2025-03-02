#pragma once

#include <stdexcept>

class UnknownTokenSubType : public std::logic_error {
    using std::logic_error::logic_error;
};

class UnknownIndentType : public UnknownTokenSubType {
    using UnknownTokenSubType::UnknownTokenSubType;
};
