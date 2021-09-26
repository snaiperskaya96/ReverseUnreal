add_library(ReverseUnrealLoader SHARED Loader.cpp)
target_compile_definitions(ReverseUnrealLoader PRIVATE -DBINARY_DIR=${CMAKE_BINARY_DIR})