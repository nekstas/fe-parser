#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

template <typename T>
class ContainerStream {
    using ElementType = std::remove_reference_t<decltype(T()[0])>;

public:
    ContainerStream() = default;
    explicit ContainerStream(const T& values) : values_(values) {
    }

    bool Eof() const {
        return pos_ >= values_.size();
    }

    ElementType Peek() const {
        return Peek(0);
    }

    ElementType Peek(size_t offset) const {
        size_t new_pos = pos_ + offset;
        if (new_pos >= values_.size()) {
            return ElementType();
        }
        return values_[new_pos];
    }

    ElementType Get() {
        return values_[pos_++];
    }

    ElementType SafeGet() {
        if (pos_ >= values_.size()) {
            return ElementType();
        }
        return values_[pos_++];
    }

    void Ignore() {
        ++pos_;
    }

    void Skip(size_t offset) {
        pos_ += offset;
    }

    void Restore(size_t pos) {
        pos_ = pos;
    }

    size_t GetPos() const {
        return pos_;
    }

private:
    T values_;
    size_t pos_ = 0;
};
