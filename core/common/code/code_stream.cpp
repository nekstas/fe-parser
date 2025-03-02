#include "code_stream.h"

CodeStream::CodeStream(const std::string &code) : code_(code) {
}

bool CodeStream::Eof() const {
    return pos_ >= code_.size();
}

char CodeStream::Peek() const {
    return Peek(0);
}

char CodeStream::Peek(size_t offset) const {
    size_t new_pos = pos_ + offset;
    if (new_pos >= code_.size()) {
        return '\0';
    }
    return code_[new_pos];
}

char CodeStream::Get() {
    return code_.at(pos_++);
}

void CodeStream::Ignore() {
    ++pos_;
}

void CodeStream::Skip(size_t offset) {
    pos_ += offset;
}

void CodeStream::Restore(size_t pos) {
    pos_ = pos;
}

size_t CodeStream::GetPos() const {
    return pos_;
}
