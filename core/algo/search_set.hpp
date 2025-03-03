#include <exception>
#include <set>

#include "../../utils/format_stream.h"

class SearchSetError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class KeyAlreadyExistsError : public SearchSetError {
public:
    KeyAlreadyExistsError(const std::string& key)
        : SearchSetError(FormatStream() << "Key " << key << " already exists in a search set.") {
    }
};

class SearchSet {
public:
    SearchSet() {
    }

    void Add(const std::string& key) {
        if (Contains(key)) {
            throw KeyAlreadyExistsError{key};
        }
        values_.insert(key);
    }

    bool HasPrefix(const std::string& prefix) const {
        for (const std::string& key : values_) {
            if (key.starts_with(prefix)) {
                return true;
            }
        }
        return false;
    }

    bool Contains(const std::string& key) const {
        return values_.contains(key);
    }

private:
    std::set<std::string> values_;
};