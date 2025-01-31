#pragma once

#include <stdexcept>

class UnknownOperatorType : public std::logic_error {
    using std::logic_error::logic_error;
};
