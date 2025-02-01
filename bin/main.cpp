#include <cstdlib>
#include <iostream>
#include <cstdio>

#include "../lib/storage/storage.h"
#include "../lib/bmp/bmp.h"
#include "../lib/parser/parser.h"


signed main(int32_t argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: ./bin/labwork3 --input=file.tsv --output=output_directory/ --max-iter=iterations --freq=frequency";
        std::exit(EXIT_FAILURE);
    }

    Parser cmd(argc, argv);
    cmd.ParseInputFile();
    cmd.ParseOutputDirectory();
    cmd.ParseMaxIter();
    cmd.ParseFrequency();
    int64_t max_iter = cmd.GetMaxIter();
    int64_t frequency = cmd.GetFrequency();
    const char* input_filename = cmd.GetInputFilename();
    const char* output_directory = cmd.GetOutputDirectory();
    HashTable field;
    Stack left_stack;
    Stack right_stack;
    field.GetInputValues(input_filename,left_stack);
    int64_t cnt = 1;

    while (!left_stack.IsEmpty() && max_iter > 0) {
        if (frequency != 0 && cnt % frequency == 0) {
            SaveBMP(field, cnt / frequency, output_directory);
        }
        field.TopplingIteration(left_stack, right_stack);
        --max_iter;
        ++cnt;
    }
    SaveBMP(field, cnt, output_directory);

    return 0;
}