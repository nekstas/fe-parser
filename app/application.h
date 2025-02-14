#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

class Application {
public:
    Application(size_t argc, char** argv);

    int32_t Run();

private:
    std::string code_;
};
