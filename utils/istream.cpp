#include "istream.h"

IStream::IStream(std::istream &stream) : stream_(stream) {
}

bool IStream::Eof() {
    return stream_.eof();
}

int32_t IStream::Peek() {
    return stream_.peek();
}

int32_t IStream::Peek(size_t rel_pos) {
    stream_.seekg(rel_pos, std::ios::cur);
    int c = Peek();
    stream_.seekg(-rel_pos, std::ios::cur);
    return c;
}

int32_t IStream::Get() {
    return stream_.get();
}

void IStream::Ignore() {
    stream_.ignore();
}

void IStream::Skip(size_t rel_pos) {
    stream_.seekg(rel_pos, std::ios::cur);
}
