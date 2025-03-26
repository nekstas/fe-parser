#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

class Application {
private:
    static constexpr size_t kMinArgsCount = 2;
    static constexpr size_t kMaxArgsCount = 3;

public:
    Application() = default;

    void PrintHelpMessage();

    void FormatFile(const std::string& input_file, const std::string& output_file);

    int32_t Run(size_t argc, char** argv);
};
