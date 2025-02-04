#pragma once
#include <cstdint>
#include <cstddef>

class Application {
public:
    Application(size_t argc, char** argv);

    int32_t Run();
};
