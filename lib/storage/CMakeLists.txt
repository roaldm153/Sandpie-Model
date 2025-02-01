add_library(
    storage
    storage.cpp
    storage.h
)

target_include_directories(storage PUBLIC &{PROJECT_SOURCE_DIR}/lib)

target_link_libraries(storage PRIVATE bmp)
target_link_libraries(storage PRIVATE parser)