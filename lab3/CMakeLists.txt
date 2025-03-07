add_executable(lab3 main.cpp)

target_compile_features(lab3 PUBLIC cxx_std_17)

target_include_directories(lab3 PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(lab3
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        NO
)
