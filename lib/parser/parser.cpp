#include <cstdlib>
#include <iostream>
#include <cstring>
#include <charconv>

#include "parser.h"    


void Parser::ParseInputFile() {
    const char* kInputLongKey = "--input=";
    const char* kInputShortKey = "-i";
    for (std::size_t i = 0; i < argc_; ++i) {
        if (strncmp(argv_[i], kInputLongKey, strlen(kInputLongKey)) == 0) {
            input_filename_ = (argv_[i] + strlen(kInputLongKey));

            return;
        }
        if (strncmp(argv_[i], kInputShortKey, strlen(kInputShortKey)) == 0) {
            if (i + 1 < argc_) {
                input_filename_ = argv_[i + 1];

                return;
            }
        }
    }
 }

void Parser::ParseOutputDirectory() {
    const char* kOutputLongKey = "--output=";
    const char* kOutputShortKey = "-o";
    for (std::size_t i = 0; i < argc_; ++i) {
        if (strncmp(argv_[i], kOutputLongKey, strlen(kOutputLongKey)) == 0) {
            output_directory_ = (argv_[i] + strlen(kOutputLongKey));
            
            return;
        }
        if (strncmp(argv_[i], kOutputShortKey, strlen(kOutputShortKey)) == 0) {
            if (i + 1 < argc_) {
                output_directory_ = argv_[i + 1];
                
                return;
            }
        }
    }
}

bool TryParseUInt64T(char* str, uint64_t& dest) {
  char* str_end = str + std::strlen(str);
  auto parse_result = std::from_chars(str, str_end, dest);

  if (parse_result.ec == std::errc::invalid_argument) {
    std::cerr << "Invalid argument: " << str << " isn't a number." << std::endl;

    return false;
  }
  if (parse_result.ec == std::errc::result_out_of_range) {
    std::cerr << "Invalid argument: " << str << " out of range." << std::endl;

    return false;
  }
  if (parse_result.ec == std::errc()) {
    if (parse_result.ptr != str_end) {
      std::cerr << "Invalid argument: " << str << " isn't a number." << std::endl;

      return false;
    }

    return true;
  }

  return false;
}

void Parser::ParseMaxIter() {
    const char* kMaxIterLongKey = "--max-iter=";
    const char* kMaxIterShortKey = "-m";
    for (std::size_t i = 0; i < argc_; ++i) {
        if (strncmp(argv_[i], kMaxIterLongKey, strlen(kMaxIterLongKey)) == 0) {
            if (!TryParseUInt64T((argv_[i] + strlen(kMaxIterLongKey)), max_iter)) {
                std:exit(EXIT_FAILURE);
            }
            
            return;
        }
        if (strncmp(argv_[i], kMaxIterShortKey, strlen(kMaxIterShortKey)) == 0) {
            if (i + 1 < argc_) {
                if (!TryParseUInt64T(argv_[i + 1], max_iter)) {
                    std::exit(EXIT_FAILURE);
                }
                
                return;
            }
        }
    }
}

void Parser::ParseFrequency() {
    const char* kFreqLongKey = "--freq=";
    const char* kFreqShortKey = "-f";
    for (std::size_t i = 0; i < argc_; ++i) {
        if (strncmp(argv_[i], kFreqLongKey, strlen(kFreqLongKey)) == 0) {
            if (!TryParseUInt64T((argv_[i] + strlen(kFreqLongKey)), frequency)) {
                std:exit(EXIT_FAILURE);
            }
            
            return;
        }
        if (strncmp(argv_[i], kFreqShortKey, strlen(kFreqShortKey)) == 0) {
            if (i + 1 < argc_) {
                if (!TryParseUInt64T(argv_[i + 1], frequency)) {
                    std::exit(EXIT_FAILURE);
                }
                
                return;
            }
        }
    }
}

int64_t Parser::GetMaxIter() {
    return max_iter;
}

int64_t Parser::GetFrequency() {
    return frequency;
}

const char* Parser::GetInputFilename() {
    return input_filename_;
}

const char* Parser::GetOutputDirectory() {
    return output_directory_;
}
