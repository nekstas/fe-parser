#pragma once

#include <cstdint>
#include <iostream>

class CodeStream {
public:
    CodeStream(const std::string &code);

    bool Eof();

    char Peek();

    char Peek(size_t offset);

    char Get();

    void Ignore();

    void Skip(size_t offset);

private:

private:
    const std::string &code_;
    size_t pos_;
};

