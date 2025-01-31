#pragma once
#include <cstdint>
#include <iostream>

class IStream {
public:
    IStream(std::istream& stream);

    bool Eof();

    int32_t Peek();

    int32_t Peek(size_t rel_pos);

    int32_t Get();

    void Ignore();

    void Skip(size_t rel_pos);

private:
    std::istream& stream_;
};
