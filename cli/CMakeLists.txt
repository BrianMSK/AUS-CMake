add_executable(main main.cpp)

target_link_libraries(main PRIVATE ds tests complexities)

target_compile_definitions(main PRIVATE ANALYZER_OUTPUT="${PROJECT_SOURCE_DIR}")

set_target_properties(main
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        NO
)
