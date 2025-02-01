add_library(
    parser
    parser.cpp 
    parser.h
)

target_include_directories(parser PUBLIC &{PROJECT_SOURCE_DIR}/lib)

target_link_libraries(parser PRIVATE storage)
target_link_libraries(parser PRIVATE bmp)