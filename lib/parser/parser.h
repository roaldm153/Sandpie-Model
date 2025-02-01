#pragma once
#include <cstdint>
#include <initializer_list>
#include <utility>

class Parser {
private:
    int32_t argc_;
    char** argv_ = nullptr;
    const char* input_filename_ = nullptr;
    const char* output_directory_ = nullptr;
    uint64_t max_iter;
    uint64_t frequency;
public:
    Parser(int32_t argc, char** argv): argc_(argc), argv_(argv) {};
    void ParseInputFile();
    void ParseOutputDirectory();
    void ParseMaxIter();
    void ParseFrequency();
    int64_t GetMaxIter();
    int64_t GetFrequency();
    const char* GetInputFilename();
    const char* GetOutputDirectory();
};