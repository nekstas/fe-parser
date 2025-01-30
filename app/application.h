#pragma once
#include <cstdint>

class Application {
public:
    Application(size_t argc, char** argv);

    int32_t Run();
};
