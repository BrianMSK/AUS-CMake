add_library(complexities SHARED complexity_analyzer.cpp)

target_compile_features(complexities PUBLIC cxx_std_17)

target_include_directories(complexities PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(complexities
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        NO
)
