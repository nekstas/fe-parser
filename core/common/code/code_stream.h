#pragma once

#include <cstdint>
#include <iostream>

class CodeStream {
public:
    CodeStream(const std::string &code);

    bool Eof() const;

    char Peek() const;

    char Peek(size_t offset) const;

    char Get();

    void Ignore();

    void Skip(size_t offset);

    size_t GetPos() const;

    void Restore(size_t pos);

private:
    const std::string &code_;
    size_t pos_ = 0;
};

