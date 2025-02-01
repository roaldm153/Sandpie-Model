add_library(
    bmp
    bmp.cpp
    bmp.h
)

target_include_directories(bmp PUBLIC &{PROJECT_SOURCE_DIR}/lib)

target_link_libraries(bmp PRIVATE storage)
target_link_libraries(bmp PRIVATE parser)